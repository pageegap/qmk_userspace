#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define STATIC_ASSERT _Static_assert
#define EECONFIG_USER_DATA_SIZE 256
typedef struct { int8_t x, y, h, v; uint8_t buttons; } report_mouse_t;
bool eeconfig_is_user_datablock_valid(void);
uint32_t eeconfig_read_user_datablock(void *data, uint32_t offset, uint32_t length);
uint32_t eeconfig_update_user_datablock(const void *data, uint32_t offset, uint32_t length);
