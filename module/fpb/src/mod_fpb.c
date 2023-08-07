/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mod_fpb.h>
#include <fwk_log.h>
#include <fwk_mm.h>
#include <fmw_cmsis.h>
#include <fwk_module.h>
#include <stdbool.h>
#include <stddef.h>

#define FPB_VERSION		2


typedef struct
{
	__IOM uint32_t FP_CTRL;
	__IOM uint32_t FP_REMAP;
	__IOM uint32_t *FP_COMP;
} FPB_Type;

#define FPB_BASE	(SCS_BASE + 0x2000UL)
#define FPB			((FPB_Type *)FPB_BASE)

/* FP_CTRL */
#define FP_CTRL_ENABLE_Pos		0U
#define FP_CTRL_ENABLE_Msk		(1UL /*<< FP_CTRL_ENABLE_Pos*/)
#define FP_CTRL_KEY_Pos 		1U
#define FP_CTRL_KEY_Msk			(1UL << FP_CTRL_KEY_Pos)
#define FP_CTRL_NUM_CODE_L_Pos 	4U
#define FP_CTRL_NUM_CODE_L_Msk 	(0xFUL << FP_CTRL_NUM_CODE_L_Pos)
#define FP_CTRL_NUM_CODE_H_Pos 	12U
#define FP_CTRL_NUM_CODE_H_Msk 	(0x7UL << FP_CTRL_NUM_CODE_H_Pos)
#define FP_CTRL_NUM_CODE_SHIFT 	4U
#define FP_CTRL_NUM_LIT_Pos 	8U
#define FP_CTRL_NUM_LIT_Msk 	(0xFUL << FP_CTRL_NUM_LIT_Pos)
#define FP_CTRL_REV_Pos			28U
#define FP_CTRL_REV_Msk			(0xFUL << FP_CTRL_REV_Pos)

/* FP_COMPn */
#define FP_COMP_ENABLE_Pos		0
#define FP_COMP_ENABLE_Msk		(1UL /*<< FP_COMP_ENABLE_Pos*/)
#if FPB_VERSION == 1
#define FP_COMP_COMP_Pos		2
#define FP_COMP_COMP_Msk		(0x7FFFFFFUL << FP_COMP_COMP_Pos)
#define FP_COMP_REPLACE_Pos		30
#define FP_COMP_REPLACE_RMP		(0UL << FP_COMP_REPLACE_Pos)
#define FP_COMP_REPLACE_BLO		(1UL << FP_COMP_REPLACE_Pos)
#define FP_COMP_REPLACE_BHI		(2UL << FP_COMP_REPLACE_Pos)
#define FP_COMP_REPLACE_BHL		(3UL << FP_COMP_REPLACE_Pos)
#elif FPB_VERSION == 2
#define FP_COMP_COMP_Msk		(~1UL)
#endif

#define LAR_OFFSET	0xFB0
#define LAR_KEY		0xC5ACCE55
#define unlock_coresight_component(base) \
	*(__O uint32_t *)(base + LAR_OFFSET) = LAR_KEY
#define lock_coresight_component(base) \
	*(__O uint32_t *)(base + LAR_OFFSET) = 0

#define unlock_fpb()	unlock_coresight_component(FPB_BASE)
#define lock_fpb()		lock_coresight_component(FPB_BASE)


#define THUMB_INST_MSK			(~1UL)


static uint8_t fpb_num_code;
static breakpoint_t *breakpoint_pool;

static int fpb_init(fwk_id_t module_id,
                     unsigned int element_count,
                     const void *data)
{
	uint32_t tmp;
	int i;
	breakpoint_t *b;

	tmp = FPB->FP_CTRL;
	if ((tmp & FP_CTRL_REV_Msk) >> FP_CTRL_REV_Pos != FPB_VERSION - 1) {
		FWK_LOG_ERR("FPB version error\n");
		return -1;
	}

	fpb_num_code = ((tmp & FP_CTRL_NUM_CODE_L_Msk) >> FP_CTRL_NUM_CODE_L_Pos) |
					((tmp & FP_CTRL_NUM_CODE_H_Msk) >> (FP_CTRL_NUM_CODE_H_Pos -
					FP_CTRL_NUM_CODE_SHIFT));	
	b = fwk_mm_alloc(fpb_num_code, sizeof(b[0]));

	unlock_fpb();
	FPB->FP_CTRL = FP_CTRL_KEY_Msk & ~FP_CTRL_ENABLE_Msk;
	for (i = 0; i < fpb_num_code; i++) {
		b[i].en = 0;
		FPB->FP_COMP[i] = 0;
	}

	lock_fpb();
	breakpoint_pool = b;
	fpb_test();

	return 0;
}

int fpb_enable(void)
{
	unlock_fpb();
	FPB->FP_CTRL = FP_CTRL_KEY_Msk | FP_CTRL_ENABLE_Msk;
	lock_fpb();
	if ((FPB->FP_CTRL & FP_CTRL_ENABLE_Msk) == 0) {
		FWK_LOG_ERR("FPB enable failed\n");
		return -1;
	}

	CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk;

	return 0;
}

static int alloc_breakpoint(void)
{
	int i;

	for (i = 0; i < fpb_num_code; i++)
		if (breakpoint_pool[i].en == 0)
			return i;

	return -1;
}

int set_breakpoint(uint32_t addr, pFunc hook, reg_patch_t *rpp)
{
	int i;
	breakpoint_t *b;

	i = alloc_breakpoint();
	if (i < 0) {
		FWK_LOG_ERR("No available breakpoints\n");
		return -1;
	}

	b = &breakpoint_pool[i];
	b->addr = addr & THUMB_INST_MSK;
	b->hook = hook;
	b->rp = *rpp;
	b->en = 1;

	unlock_fpb();
#if FPB_VERSION == 1
	if (b->addr & 0x2UL)
		FPB->FP_COMP[i] = FP_COMP_REPLACE_BHI |
							(b->addr & FP_COMP_COMP_Msk) | FP_COMP_ENABLE_Msk;
	else
		FPB->FP_COMP[i] = FP_COMP_REPLACE_BLO |
							(b->addr & FP_COMP_COMP_Msk) | FP_COMP_ENABLE_Msk;
#elif FPB_VERSION == 2
	FPB->FP_COMP[i] = (b->addr & FP_COMP_COMP_Msk) | FP_COMP_ENABLE_Msk;
#endif
	lock_fpb();

	return 0;
}

breakpoint_t *get_breakpoint(uint32_t addr)
{
	int i;
	uint32_t iaddr = addr & THUMB_INST_MSK;

	for (i = 0; i < fpb_num_code; i++) {
		if (breakpoint_pool[i].en == 0)
			break;
		if (breakpoint_pool[i].addr == iaddr)
			return &breakpoint_pool[i];
	}

	return NULL;
}

const struct fwk_module_config config_fpb = { 0 };
const struct fwk_module module_fpb = {
    .type = FWK_MODULE_TYPE_DRIVER,
    .init = fpb_init,
};

void fpb_trigger(void)
{
	FWK_LOG_INFO("%s\n", __func__);
}

void fpb_hook(void)
{
	FWK_LOG_INFO("%s\n", __func__);
}

uint32_t test_reg[4];
reg_t reg_patch_buf[] = {
	[0] = {
		.addr = (uint32_t)&test_reg[0],
		.value = 0x55AA,
	},
	[1] = {
		.addr = (uint32_t)&test_reg[3],
		.value = 0x11223344,
	},
};

void fpb_test()
{
	reg_patch_t rp;
	rp.num = 2;
	rp.regs = reg_patch_buf;
	set_breakpoint((uint32_t)fpb_trigger, fpb_hook, &rp);
	fpb_enable();
	__asm("isb");
	fpb_trigger();
}

