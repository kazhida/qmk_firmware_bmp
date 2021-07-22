/* Copyright 2019 sekigon-gonnoc
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
#include "pointing_device.h"
#include "bmp.h"
#include "bmp_custom_keycode.h"
#include "keycode_str_converter.h"

uint8_t tb_button    = 0;
uint8_t tb_scrl_flag = false;

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    CUSTOM_KC = BMP_SAFE_RANGE,
};

const key_string_map_t custom_keys_user = {.start_kc    = CUSTOM_KC,
                                           .end_kc      = CUSTOM_KC,
                                           .key_strings = "\0"};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {KC_NO};

uint32_t keymaps_len() { return 43; }

bool process_record_user(uint16_t keycode, keyrecord_t *record) { return false; }

void matrix_init_user(void) {}

void matrix_scan_user(void) {}
