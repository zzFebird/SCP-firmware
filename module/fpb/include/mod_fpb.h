
/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_FPB_H
#define MOD_FPB_H

#include <stdint.h>

typedef struct
{
	uint32_t    addr;
	uint32_t    value;
} reg_t;

typedef struct
{
	uint8_t		num;
	reg_t		*regs;
} reg_patch_t;

typedef void (*pFunc)(void);

typedef struct
{
	uint32_t	addr;
	reg_patch_t	rp;
	pFunc		hook;
	uint8_t		en;
} breakpoint_t;

int fpb_enable(void);
int set_breakpoint(uint32_t addr, pFunc hook, reg_patch_t *rp);
breakpoint_t *get_breakpoint(uint32_t addr);
void fpb_test();

#endif /* MOD_FPB_H */
