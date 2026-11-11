// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "bk_pointing_device.h"
#include "bk_pointing_storage.h"

extern bkpd_config_t g_bkpd_config;
STATIC_ASSERT(sizeof(bkpd_config_t) <= EECONFIG_USER_DATA_SIZE,
              "Pointing settings exceed user EEPROM");

void read_bkpd_config_from_eeprom(void) {
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&g_bkpd_config, 0, sizeof(g_bkpd_config));
    }
}

void write_bkpd_config_to_eeprom(void) {
    eeconfig_update_user_datablock(&g_bkpd_config, 0, sizeof(g_bkpd_config));
}
