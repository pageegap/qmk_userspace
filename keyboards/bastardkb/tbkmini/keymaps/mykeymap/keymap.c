/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
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


enum custom_layers {
     _QWERTY,
     _LOWER,
     _RAISE,
     _MOVE
};

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

// Tap Dance declarations1234567890             
enum {
    TD_SHOW_SCREEN,
};

const uint16_t PROGMEM test_combo1[] = {QK_GESC, KC_DEL, COMBO_END};

combo_t key_combos[] = {
    COMBO(test_combo1, LCTL(LGUI(KC_Q))), // keycodes with modifiers are possible too!
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHOW_SCREEN] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_UP), LGUI(KC_UP)),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_split_3x6_3(                     
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
            KC_CAPS,   KC_Q,   KC_W,   KC_E,     KC_R,    KC_T,                          KC_Y,   KC_U,    KC_I,    KC_O,     KC_P,  KC_ENT,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
          KC_BSPC, LCMD_T(KC_A), LOPT_T(KC_S), CTL_T(KC_D), KC_F, KC_G,              KC_H, KC_J, RCTL_T(KC_K),ROPT_T(KC_L),RCMD_T(KC_SCLN), KC_QUOT,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
          KC_LSFT, LT(_LOWER, KC_Z), KC_X, KC_C, KC_V,  LT(_MOVE, KC_B),                 KC_N,   KC_M,   KC_COMM, KC_DOT,  LT(_RAISE, KC_SLSH), KC_RSFT,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        TD(TD_SHOW_SCREEN), KC_SPC, QK_GESC,                                                       KC_DEL, KC_SPC,  KC_TAB                  
        //`--------------------------'  `--------------------------'

        ),

    [_LOWER] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
            _______, _______, _______, _______, _______, _______,                      _______, KC_1,   KC_2,   KC_3,  _______,  _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
            _______, _______, _______, RCS(KC_TAB), LCTL(KC_TAB), _______,             _______, KC_4,   KC_5,   KC_6,   KC_UNDO, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
            _______, _______,  KC_LGUI, _______, _______, _______,                      _______,  KC_7,   KC_8,   KC_9,   KC_0,  _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                            _______, KC_TAB, _______, _______, _______, _______
        //`--------------------------'  `--------------------------'
        ),

    [_RAISE] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         _______, _______, KC_PERCENT, RSFT(KC_LBRC), RSFT(KC_RBRC), KC_HASH,          _______, KC_AMPR, KC_ASTR, KC_GRV, _______, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         _______, _______, KC_AT,  KC_LPRN, KC_RPRN, KC_TILD,                          KC_EXLM ,KC_EQL, KC_PLUS, KC_MINUS, _______, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, _______, KC_CIRC, KC_LBRC, KC_RBRC, KC_DLR,                           _______, KC_PIPE,  _______, KC_LGUI, _______, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______, _______, _______,                   _______, KC_TAB, _______
        //`--------------------------'  `--------------------------'
        ),

    [_MOVE] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         RGB_MOD,   RGB_TOG, _______, _______,_______, _______,                        _______,  KC_MUTE,KC_VOLU , KC_VOLD ,_______, QK_BOOT,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
         _______,  KC_F12,  KC_F7  , KC_F8 ,  KC_F10  , _______,                       _______, KC_LEFT,  KC_UP,  KC_DOWN,  KC_RGHT, _______,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______,  _______, _______, _______,_______, _______,                          _______,  NEWLINE, KC_HOME, KC_END,_______, _______,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______, _______, _______, _______ , KC_LOPT, _______
        //`--------------------------'  `--------------------------'
        )};
