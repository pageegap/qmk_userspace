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
#include QMK_KEYBOARD_H
#include "keymap_us_international.h"
#include "sendstring_us_international.h"

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

// my modifications

enum custom_keycodes {
    NEWLINE = SAFE_RANGE,
    PRINT_SCR
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case NEWLINE:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("e"));
                register_code(KC_ENT);
                unregister_code(KC_ENT);
            }
            return false;
    }

    return true;
}

// Tap Dance declarations      
enum {
    TD_SHOW_SCREEN
};

const uint16_t PROGMEM lockscr_combo[] = {KC_CAPS, KC_ENT, COMBO_END};
const uint16_t PROGMEM printscreen[] = { KC_G, KC_H, COMBO_END};
const uint16_t PROGMEM esc_combo[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM ent_combo[] = {KC_M, KC_COMM, COMBO_END};

combo_t key_combos[] = {
    COMBO(lockscr_combo, LCTL(LGUI(KC_Q))),
    COMBO(printscreen, SCMD(KC_5)),
    COMBO(esc_combo, KC_ESC),
    COMBO(ent_combo, KC_ENT)
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHOW_SCREEN] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_UP), LGUI(KC_UP)),
};


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
     //,-----------------------------------------------------.                    ,-----------------------------------------------------.
          KC_CAPS,   KC_Q,   KC_W,   KC_E,     KC_R,    KC_T,                          KC_Y,   KC_U,    KC_I,    KC_O,     KC_P,  KC_ENT,
     //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_BSPC, LCMD_T(KC_A), LOPT_T(KC_S), CTL_T(KC_D), KC_F, KC_G,              KC_H, KC_J, RCTL_T(KC_K),ROPT_T(KC_L),RCMD_T(KC_SCLN), KC_QUOT,
     //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, LT(LAYER_LOWER, KC_Z), KC_X, KC_C, KC_V,  LT(LAYER_POINTER, KC_B),   KC_N,   KC_M,   KC_COMM, KC_DOT,  LT(LAYER_RAISE, KC_SLSH), KC_RSFT,
     //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
     LSFT_T(KC_ESC) , LT(LAYER_POINTER, KC_BTN1),  TD(TD_SHOW_SCREEN),                 KC_DEL, RSFT_T(KC_SPC)
     //                             `--------------------------'                   `--------------------------'
  ),

  [LAYER_LOWER] = LAYOUT(
//,-----------------------------------------------------.                    ,-----------------------------------------------------.
            RGB_MOD, RGB_TOG, RGB_VAI, RGB_VAD, _______, _______,                      _______, KC_1,   KC_2,   KC_3,  _______,  _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
            _______, _______, _______, RCS(KC_TAB), LCTL(KC_TAB), _______,             _______, KC_4,   KC_5,   KC_6,   KC_UNDO, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
            _______, _______,  KC_LGUI, _______, _______, _______,                      _______,  KC_7,   KC_8,   KC_9,   KC_0,  _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
            _______, KC_TAB, _______, _______, _______
        //`--------------------------'  `--------------------------'

  ),

  [LAYER_RAISE] = LAYOUT(
     //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         _______, _______, KC_PERCENT, RSFT(KC_LBRC), RSFT(KC_RBRC), KC_HASH,          KC_PIPE, KC_AMPR, KC_ASTR, KC_GRV, _______, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         _______, _______, KC_AT,  KC_LPRN, KC_RPRN, KC_TILD,                          KC_EXLM ,KC_EQL, KC_PLUS, KC_MINUS, _______, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, KC_BSLS, KC_CIRC, KC_LBRC, KC_RBRC, KC_DLR,                           KC_LGUI, KC_BTN1,  KC_BTN2, DRGSCRL, _______, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______, _______, _______,                   KC_BTN1, KC_TAB
        //`--------------------------'  `--------------------------'
  ),

  [LAYER_POINTER] = LAYOUT(
     //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         QK_BOOT, _______,  _______, _______,S_D_MOD, DPI_MOD,                        _______,  KC_MUTE,KC_VOLU , KC_VOLD ,_______, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         _______,  KC_F12,  KC_F7  , KC_F8 ,  KC_F10  , _______,                       KC_BTN5, KC_LEFT,  KC_UP,  KC_DOWN,  KC_RGHT, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______,  DRGSCRL, KC_BTN4, KC_BTN2 , KC_BTN1, KC_BTN5,                        KC_BTN4,  NEWLINE, KC_HOME, KC_END,SNIPING, DRGSCRL,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______, _______, _______,         KC_BTN1 , KC_BTN2
        //`--------------------------'  `--------------------------'
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
