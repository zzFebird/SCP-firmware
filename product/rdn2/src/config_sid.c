/*
 * Arm SCP/MCP Software
 * Copyright (c) 2020-2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "scp_css_mmap.h"

#include <mod_sid.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>

static const struct fwk_element subsystem_table[] = {
    { .name = "RD-N2",
      .data =
          &(struct mod_sid_subsystem_config){
#if (PLATFORM_VARIANT == 0 || PLATFORM_VARIANT == 2)
              .part_number = 0x7B7,
#elif (PLATFORM_VARIANT == 3)
              .part_number = 0x7F1,
#else
              .part_number = 0x7B6,
#endif
          } },
    { .name = "RD-V2",
      .data =
          &(struct mod_sid_subsystem_config){
              .part_number = 0x7F2,
          } },
    { 0 },
};

static const struct fwk_element *get_subsystem_table(fwk_id_t id)
{
    return subsystem_table;
}

const struct fwk_module_config config_sid = {
    .data = &(struct mod_sid_config) {
        .sid_base = SCP_SID_BASE,
        .pcid_expected = {
            .PID0 = 0xD2,
            .PID1 = 0xB0,
            .PID2 = 0x0B,
            .PID3 = 0x00,
            .PID4 = 0x04,
            .CID0 = 0x0D,
            .CID1 = 0xF0,
            .CID2 = 0x05,
            .CID3 = 0xB1,
        },
    },

    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(get_subsystem_table),
};
