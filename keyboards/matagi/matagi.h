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
#pragma once

#include "quantum.h"


enum {
    CMD_INPUT = 0,
    CMD_OUTPUT,
    CMD_INVERSION,
    CMD_CONFIG,
};

typedef struct {
    uint8_t address;
    uint8_t num_port;
    uint16_t mask;
} expander;

#define PCA9555(offset) { 0x20 | offset, 2, 0xFFFF }
#define TCA9555(offset) { 0x20 | offset, 2, 0xFFFF }
#define PCA9554(offset) { 0x20 | offset, 1, 0x00FF }
#define TCA9554(offset) { 0x20 | offset, 1, 0x00FF }
#define PCA9536() { 0x41, 1, 0x000F }

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    K_ESC ,  K_1, K_2, K_3, K_4, K_5,        K_6, K_7, K_8, K_9, K_0, K_MNS, K_EQL, K_BSLS, K_GRV,  \
    K_TAB,  K_Q, K_W, K_E, K_R, K_T,        K_Y, K_U, K_I, K_O, K_P, K_LB, K_RB, K_BS,              \
    K_CAPS, K_A, K_S, K_D, K_F, K_G,        K_H, K_J, K_K, K_L, K_SCLN, K_QUOT, K_ENT,              \
    K_LSFT, K_Z, K_X, K_C, K_V, K_B,        K_N, K_M, K_COM, K_DOT, K_SLS, K_RSFT, K_UP, K_DEL,     \
    K_LCTL, K_LGUI, K_FN, K_LALT, K_SPC0,   K_SPC1, K_RALT, K_APP, K_LEFT, K_DOWN, K_RGHT           \
) {                                                                                                 \
    { K_ESC, K_1, K_2, K_3,     /**/ K_4, K_5, K_6, K_7,            /**/ K_8, K_9, K_0, K_MNS,          /**/ K_EQL, K_BSLS, K_GRV, KC_NO }, \
    { K_TAB,  K_Q, K_W, K_E,    /**/ K_R, K_T, KC_NO, KC_NO,        /**/ K_CAPS, K_A, K_S, K_D,         /**/ K_F, K_G, KC_NO, KC_NO },      \
    { K_Y, K_U, K_I, K_O,       /**/ K_P, K_LB, K_RB, K_BS,         /**/ K_H, K_J, K_K, K_L,            /**/ K_SCLN, K_QUOT, K_ENT, KC_NO },\
    { K_LSFT, K_Z, K_X, K_C,    /**/ K_V, K_B, KC_NO, KC_NO,        /**/ K_LCTL, K_LGUI, K_FN, K_LALT,  /**/ K_SPC0, K_SPC1, KC_NO, KC_NO },\
    { K_N, K_M, K_COM, K_DOT,   /**/ K_SLS, K_RSFT, K_UP, K_DEL,    /**/ K_RALT, K_APP, K_LEFT, K_DOWN, /**/ K_RGHT, KC_NO, KC_NO, KC_NO }  \
}
