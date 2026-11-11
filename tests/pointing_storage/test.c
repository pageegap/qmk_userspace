#include "fake_qmk.h"
#include "bk_pointing_device.h"
#include "storage_under_test.inc"
#include <assert.h>
#include <stdio.h>
#include <string.h>

bkpd_config_t g_bkpd_config;
static uint8_t storage[EECONFIG_USER_DATA_SIZE];
static bool valid;
static uint32_t written;
bool eeconfig_is_user_datablock_valid(void) { return valid; }
uint32_t eeconfig_read_user_datablock(void *data, uint32_t offset, uint32_t length) {
    assert(offset + length <= sizeof(storage));
    memcpy(data, storage + offset, length);
    return length;
}
uint32_t eeconfig_update_user_datablock(const void *data, uint32_t offset, uint32_t length) {
    assert(offset + length <= sizeof(storage));
    memcpy(storage + offset, data, length);
    valid = true;
    written = length;
    return length;
}
int main(void) {
    memset(storage, 0xff, sizeof(storage));
    read_bkpd_config_from_eeprom();
    assert(!g_bkpd_config.has_copied_qmk_config); // invalid storage must be ignored
    g_bkpd_config.has_copied_qmk_config = true;
    g_bkpd_config.auto_mouse_time = 1200;
    for (uint8_t i = 0; i < MODE_LAST; ++i) {
        g_bkpd_config.modes_config[i].minimum_dpi = 900 + i * 100;
        g_bkpd_config.modes_config[i].dpi_per_step = 100;
        g_bkpd_config.modes_config[i].current_dpi_step = i + 1;
        g_bkpd_config.modes_config[i].max_dpi_steps = 20;
    }
    g_bkpd_config.custom_modes_config[4].keycode_right = 0x1234;
    bkpd_config_t expected = g_bkpd_config;
    write_bkpd_config_to_eeprom();
    assert(written == sizeof(expected));
    memset(&g_bkpd_config, 0, sizeof(g_bkpd_config)); // simulate losing RAM on reconnect
    read_bkpd_config_from_eeprom();
    assert(memcmp(&g_bkpd_config, &expected, sizeof(expected)) == 0);
    g_bkpd_config.modes_config[MODE_SNIPING].current_dpi_step = 3;
    write_bkpd_config_to_eeprom();
    memset(&g_bkpd_config, 0, sizeof(g_bkpd_config));
    read_bkpd_config_from_eeprom();
    assert(g_bkpd_config.modes_config[MODE_SNIPING].current_dpi_step == 3);
    puts("PASS: invalid EEPROM ignored; full settings and updated DPI survive reload");
}
