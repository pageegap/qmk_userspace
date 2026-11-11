/**
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 4
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif // POINTING_DEVICE_ENABLE

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE

// Encoder R1 and R2.
#define ENCODERS_PAD_A { GP20 }
#define ENCODERS_PAD_B { GP23 }
#define ENCODERS_PAD_A_RIGHT { NO_PIN }
#define ENCODERS_PAD_B_RIGHT { NO_PIN }
#define ENCODER_RESOLUTIONS_RIGHT {  }
#define ENCODER_DIRECTION_FLIP
#define ENCODER_RESOLUTION 2

// Custom DPI.
#define BK_POINTING_DEVICE_MINIMUM_DEFAULT_DPI 1200
#define BK_POINTING_DEVICE_DEFAULT_DPI_CONFIG_STEP 200
#define BK_POINTING_DEVICE_MINIMUM_SNIPING_DPI 900
#define BK_POINTING_DEVICE_SNIPING_DPI_CONFIG_STEP 100


/* RGB matrix support. */
#ifdef RGB_MATRIX_ENABLE
//#  define RGB_MATRIX_LED_COUNT 52
//#  define RGB_MATRIX_SPLIT { 32, 20 }
#  define SPLIT_TRANSPORT_MIRROR
#endif
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 130

// Reserve persistent pointing settings without the Argos EEPROM layout.
#define EECONFIG_USER_DATA_SIZE 256
#define EECONFIG_USER_DATA_VERSION 1

#ifdef AUTO_MOUSE_DEFAULT_LAYER
#    undef AUTO_MOUSE_DEFAULT_LAYER
#endif
#define AUTO_MOUSE_DEFAULT_LAYER 3

#ifndef LED_DPI_INDICATOR_INDEX
#    define LED_DPI_INDICATOR_INDEX 0
#endif
#ifndef RGBLIGHT_LED_COUNT
#    define RGBLIGHT_LED_COUNT 41
#endif

// Largest step indices: 16 normal values, four precision values.
#define BK_POINTING_DEVICE_DPI_STEPS 15
#define BK_POINTING_DEVICE_SNIPING_DPI_STEPS 3
