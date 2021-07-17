/*
Copyright 2019 sekigon-gonnoc

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef __ASSEMBLER__
#include "config_common.h"
#include "apidef.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xAB01
#define PRODUCT_ID      0x6D74
#define DEVICE_VER      0x0001
#define MANUFACTURER    ABplus Inc.
#define PRODUCT         matagi-BMP
#define DESCRIPTION     ABplus matagi

#define BMP_BOOTPIN_AS_RESET

#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define PREVENT_STUCK_MODIFIERS
#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_TERM 200

/* key matrix size */
#define MATRIX_ROWS_DEFAULT 5
#define MATRIX_COLS_DEFAULT 16
#define THIS_DEVICE_ROWS 5
#define THIS_DEVICE_COLS 16
#define IS_LEFT_HAND  true
#define BMP_DEFAULT_MODE SINGLE

/*
 * Enable mass storage when the switch connected following pins is pushed on boot.
 * No warranty
*/
// #define ALLOW_MSC_ROW_PIN 7
// #define ALLOW_MSC_COL_PIN 20

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
*/
#define MATRIX_ROW_PINS { 33 } // Dummy
#define MATRIX_COL_PINS {34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34}
#define MATRIX_LAYOUT { \
        1,  2,  3,  4,  5,  6,          7,  8,  9,  10, 11, 12, 13, 14, 15, 0,  \
        17, 18, 19, 20, 21, 22,         33, 34, 35, 36, 37, 38, 39, 40, 0,      \
        25, 26, 27, 28, 29, 30,         41, 42, 43, 44, 45, 46, 47, 0,          \
        49, 50, 51, 52, 53, 54,         65, 66, 67, 68, 69, 70, 71, 72, 0,      \
        57, 58, 59, 60, 61, 62,         73, 74, 75, 76, 77                      \
        }

#define KEYMAP_PRIOR_LOCALE 0
#define KEYMAP_ASCII 0

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

#define RGBLIGHT_SPLIT
#define G00 255 // Dummy for bin/qmk config generation
#define RGB_DI_PIN G00
#ifdef RGB_DI_PIN
  #define RGBLED_NUM_DEFAULT 128
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
  #define RGBLIGHT_LIMIT_VAL 255 /* The maximum brightness level */
  #define RGBLIGHT_SLEEP  /* If defined, the RGB lighting will be switched off when the host goes to sleep */
/*== all animations enable ==*/
  #define RGBLIGHT_ANIMATIONS
/*== or choose animations ==*/
  #define RGBLIGHT_EFFECT_BREATHING
  #define RGBLIGHT_EFFECT_RAINBOW_MOOD
  #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
  #define RGBLIGHT_EFFECT_SNAKE
  #define RGBLIGHT_EFFECT_KNIGHT
  #define RGBLIGHT_EFFECT_CHRISTMAS
  #define RGBLIGHT_EFFECT_STATIC_GRADIENT
  #define RGBLIGHT_EFFECT_RGB_TEST
  #define RGBLIGHT_EFFECT_ALTERNATING
/*== customize breathing effect ==*/
  /*==== (DEFAULT) use fixed table instead of exp() and sin() ====*/
  #define RGBLIGHT_BREATHE_TABLE_SIZE 256      // 256(default) or 128 or 64
  /*==== use exp() and sin() ====*/
  #define RGBLIGHT_EFFECT_BREATHE_CENTER 1.85  // 1 to 2.7
  #define RGBLIGHT_EFFECT_BREATHE_MAX    255   // 0 to 255
#endif

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 1

#define DEBUG_ACTION

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#endif /*__ASSEMBLER__*/
