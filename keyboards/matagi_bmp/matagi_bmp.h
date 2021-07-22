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

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

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

