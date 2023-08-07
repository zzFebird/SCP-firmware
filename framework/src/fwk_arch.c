/*
 * Arm SCP/MCP Software
 * Copyright (c) 2015-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Description:
 *     Framework API for the architecture layer.
 */
#include <internal/fwk_module.h>

#include <fwk_arch.h>
#include <fwk_assert.h>

#include <arch_helpers.h>

#if FWK_HAS_INCLUDE(<fmw_arch.h>)
#    include <fmw_arch.h>
#endif

#include <internal/fwk_core.h>

#include <fwk_io.h>
#include <fwk_log.h>
#include <fwk_module.h>
#include <fwk_module_idx.h>
#include <fwk_status.h>

#include <string.h>

extern int fwk_interrupt_init(const struct fwk_arch_interrupt_driver *driver);

static int fwk_arch_interrupt_init(int (*interrupt_init_handler)(
    const struct fwk_arch_interrupt_driver **driver))
{
    /* Initialize interrupt management */
    int status;
    const struct fwk_arch_interrupt_driver *driver;

    /*
     * Retrieve a pointer to the interrupt management driver from the
     * architecture layer.
     */
    status = interrupt_init_handler(&driver);
    if (status != FWK_SUCCESS) {
        return FWK_E_PANIC;
    }

    /* Initialize the interrupt management component */
    status = fwk_interrupt_init(driver);
    if (status != FWK_SUCCESS) {
        return FWK_E_PANIC;
    }

    return FWK_SUCCESS;
}

#define write32(addr, val)	*(volatile uint32_t *)(addr) = (uint32_t)(val)
#define read32(addr)			*((volatile uint32_t *)(addr))
#define setbits32(addr, bits)	write32(addr, read32(addr) | (bits))
#define clearbits32(addr, bits)	write32(addr, read32(addr) & ~(bits))

#define BIT(x)			(1UL << x)

/*
#define FP_CTRL					0xE0002000UL
#define FP_CTRL_ENABLE_POS		0
#define FP_CTRL_ENABLE_MSK		(1UL << FP_CTRL_ENABLE_POS)
#define FP_CTRL_KEY_POS 		1
#define FP_CTRL_KEY_MSK			(1UL << FP_CTRL_KEY_POS)
#define FP_CTRL_NUM_CODE_POSL 	4
#define FP_CTRL_NUM_CODE_MSKL 	(0xFUL << FP_CTRL_NUM_CODE_POSL)
#define FP_CTRL_NUM_CODE_POSH 	12
#define FP_CTRL_NUM_CODE_MSKH 	(0x7UL << FP_CTRL_NUM_CODE_POSH)
#define FP_CTRL_NUM_LIT_POS 	8
#define FP_CTRL_NUM_LIT_MSK 	(0xFUL << FP_CTRL_NUM_LIT_POS)

#define FP_COMP(n)				(0xE0002008UL + (n << 2))

#define FP_LAR					0xE0002FB0UL
#define FP_LSR					0xE0002FB4UL
#define FP_LAR_UNLOCK_KEY		0XC5ACCE55UL

#define DEMCR			0xE000EDFCUL
#define MON_EN_POS		16
#define DCB_BASE                        (SCS_BASE + 0x0DF0)
#define DCB_DEMCR                       (volatile uint32_t *) (DCB_BASE + 0x00C)
#define SCB_ICSR                        (volatile uint32_t *) (SCB_BASE + 0x004)

void fpb_trigger(void)
{
	uint32_t pc = 0;
	__asm volatile("mov %0, pc":"=r"(pc)::);
	FWK_LOG_INFO("%s: pc = 0x%lx\n", __func__, pc);
}

void fpb_init(void)
{
	uint32_t tmp;

	write32(FP_LAR, FP_LAR_UNLOCK_KEY);
	FWK_LOG_INFO("FP_LSR = 0x%08lx\n", read32(FP_LSR));

	setbits32(FP_CTRL, FP_CTRL_ENABLE_MSK | FP_CTRL_KEY_MSK);
	FWK_LOG_INFO("FP_CTRL = 0x%08lx\n", read32(FP_CTRL));

	tmp = (uint32_t)fpb_trigger;
	FWK_LOG_INFO("tmp = 0x%lx\n", tmp);
	tmp &= FP_COMP_COMP_MSK;
	FWK_LOG_INFO("tmp = 0x%lx\n", tmp);
	tmp |= FP_COMP_ENABLE_MSK | FP_COMP_REPLACE_BB;
	FWK_LOG_INFO("tmp = 0x%lx\n", tmp);
	write32(FP_COMP(0), tmp);
	FWK_LOG_INFO("FP_COMP0 = 0x%08lx\n", read32(FP_COMP(0)));

	setbits32(FP_CTRL, FP_CTRL_ENABLE_MSK | FP_CTRL_KEY_MSK);
	FWK_LOG_INFO("FP_CTRL = 0x%08lx\n", read32(FP_CTRL));

	setbits32(DEMCR, BIT(MON_EN_POS));
	FWK_LOG_INFO("DEMCR = 0x%08lx\n", read32(DEMCR));
}

#define CLIDR		0xE000ED78UL
#define CTR			0xE000ED7CUL
#define CCSIDR		0xE000ED80UL
#define CSSELR		0xE000ED84UL
#define CSSELR_IND_POS	0
#define CSSELR_IND_MSK	(1UL << CSSELR_IND_POS)

void dump_cache_implements(void)
{
	FWK_LOG_INFO("CLIDR = 0x%08lx\n", *(uint32_t *)CLIDR);
	clearbits32(CSSELR, CSSELR_IND_MSK);
	FWK_LOG_INFO("D:CCSIDR = 0x%08lx\n", *(uint32_t *)CCSIDR);
	setbits32(CSSELR, CSSELR_IND_MSK);
	FWK_LOG_INFO("I:CCSIDR = 0x%08lx\n", *(uint32_t *)CCSIDR);
}

#define SYST_CSR	0xE000E010UL
#define SYST_RVR	0xE000E014UL
#define SYST_CVR	0xE000E018UL
#define SYST_CSR_ENABLE_POS		0
#define SYST_CSR_ENABLE_MSK		(1UL << SYST_CSR_ENABLE_POS)
#define SYST_CSR_TICKINT_POS	1
#define SYST_CSR_TICKINT_MSK	(1UL << SYST_CSR_TICKINT_POS)

void systick_init(void)
{
	write32(SYST_RVR, 0xF00000UL);
	setbits32(SYST_CSR, SYST_CSR_ENABLE_MSK | SYST_CSR_TICKINT_MSK);
	FWK_LOG_INFO("SYST_RVR = 0x%08lx\n", read32(SYST_RVR));
	FWK_LOG_INFO("SYST_CSR = 0x%08lx\n", read32(SYST_CSR));
	FWK_LOG_INFO("SYST_CVR = 0x%08lx\n", read32(SYST_CVR));
}

void systick_stop(void)
{
	clearbits32(SYST_CSR, SYST_CSR_ENABLE_MSK);
}
*/

void dump_reg32(uint32_t addr, uint32_t n)
{
    uint32_t i;

    for (i = 0; i < n; i++) {
        FWK_LOG_INFO("0x%08lX: 0x%08lx\n", addr, read32(addr));
        addr += 4;
    }
}

void run_test_code(void)
{
	//dump_cache_implements();

	//systick_init();

}

int fwk_arch_init(const struct fwk_arch_init_driver *driver)
{
    int status;
	extern void init_FPB(void);

    if (driver == NULL) {
        return FWK_E_PARAM;
    }

    if (driver->interrupt == NULL) {
        return FWK_E_PARAM;
    }

    fwk_module_init();

    status = fwk_io_init();
    if (!fwk_expect(status == FWK_SUCCESS)) {
        return FWK_E_PANIC;
    }

    status = fwk_log_init();
    if (!fwk_expect(status == FWK_SUCCESS)) {
        return FWK_E_PANIC;
    }

    /* Initialize interrupt management */
    status = fwk_arch_interrupt_init(driver->interrupt);
    if (!fwk_expect(status == FWK_SUCCESS)) {
        return FWK_E_PANIC;
    }

    status = fwk_module_start();
    if (!fwk_expect(status == FWK_SUCCESS)) {
        return FWK_E_PANIC;
    }

	run_test_code();
	while (true);

    /*
     * In case firmware running under other OS context, finish processing of
     * any raised events/interrupts and return. Else continue to process events
     * in a forever loop.
     */
#if defined(BUILD_HAS_SUB_SYSTEM_MODE)
    fwk_process_event_queue();
    fwk_log_flush();
#else
    __fwk_run_main_loop();
#endif

    return FWK_SUCCESS;
}

int fwk_arch_deinit(void)
{
    int status;

    status = fwk_module_stop();
    if (!fwk_expect(status == FWK_SUCCESS)) {
        return FWK_E_PANIC;
    }

    return FWK_SUCCESS;
}

void fwk_arch_suspend(void)
{
    /* On some arm plaforms, wfe is supported architecturally, however
     * implementation is erroneous. In such platforms FMW_DISABLE_ARCH_SUSPEND
     * needs to be defined
     */
#if !defined(FMW_DISABLE_ARCH_SUSPEND)
    arch_suspend();
#endif
}
