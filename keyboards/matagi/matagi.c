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
#include "matagi.h"
#include "config.h"
#include <string.h>
#include <i2c.h>
#include <i2c_master.h>
#include <bmp_matrix.h>
#include "debug.h"

void keyboard_post_init_user()
{
    debug_enable = true;
}

void create_user_file()
{
  static const char qmk_configurator[] = "<meta http-equiv=\"refresh\" content=\"0;URL=\'https://sekigon-gonnoc.github.io/qmk_configurator\'\"/>";
  BMPAPI->usb.create_file("MAP_EDITHTM", (uint8_t*)qmk_configurator, strlen(qmk_configurator));
}

#define TIMEOUT 100

#define ALL_OUTPUT 0
#define ALL_INPUT 0xFF
#define ALL_LOW 0
#define ALL_HIGH 0xFF

static uint8_t initialized = 0;

void expander_init(const expander *slaves, uint8_t count) {

    if (!initialized) {
        initialized++;
        i2c_init();
    }

    // make all pins to input mode
    for (uint8_t i = 0; i < count; i++) {
        uint8_t num = slaves[i].num_port;
        uint8_t addr = slaves[i].address << 1;
        for (uint8_t j = 0; j < num; j++) {
            uint8_t cmd = CMD_CONFIG * num + j;
            uint8_t conf = ALL_INPUT;
            i2c_status_t ret = i2c_writeReg(addr, cmd, &conf, sizeof(conf), TIMEOUT);
            if (ret != I2C_STATUS_SUCCESS) {
                xprintf("config pins FAILED: %d, addr: %02X, cmd: %02X conf: %d\n", ret, addr, cmd, conf);
            } else {
                xprintf("config pins SUCCEEDED: %d, addr: %02X, cmd: %02X conf: %d\n", ret, addr, cmd, conf);
            }
        }
    }
}

uint16_t expander_readPins(const expander *slave) {
    uint8_t addr = slave->address << 1;
    uint8_t data[2] = { 0xff, 0xff };     // num_port <= 2
    uint8_t cmd = CMD_INPUT * slave->num_port;

    i2c_status_t ret = i2c_readReg(addr, cmd, data, slave->num_port, TIMEOUT);
    if (ret != I2C_STATUS_SUCCESS) {
        xprintf("i2c_readReg FAILED: %d, addr: %02X, cmd: %02X\n", ret, addr, cmd);
        return 0x0000;
    } else {
        xprintf("i2c_readReg SUCCEED: %d, addr: %02X, cmd: %02X\n", ret, addr, cmd);
    }

    uint16_t result = 0;
    for (uint8_t i = slave->num_port - 1; i >= 0; i--) {
        result <<= 8;
        result |= data[i];
    }

    return (~result) & slave->mask;
}

static expander expanders[MATRIX_ROWS] = {
    PCA9555(0x01),
    PCA9555(0x02),
    PCA9555(0x03),
    PCA9555(0x04),
    PCA9555(0x05)
};

void matrix_init_custom(void) {
    debug_enable = true;

    expander_init(expanders, MATRIX_ROWS);
//    dprint("expanders initialized\n");
}

uint32_t matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;

//    dprint("matrix_scan_custom");
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix_row_t last_row = current_matrix[i];
        matrix_row_t current_row = expander_readPins(&expanders[i]);
        if (current_row != 0 && current_row != last_row) {
            xprintf("row[%d]: %04X -> %04X\n", i, last_row, current_row);
        }
        current_matrix[i] = current_row;
        changed |= (last_row != current_row);
    }

    return changed;
}

uint32_t get_rows() {
    return THIS_DEVICE_ROWS;
}

uint32_t get_cols() {
    return THIS_DEVICE_COLS;
};

static const bmp_matrix_func_t matrix_func = {
    matrix_init_custom,
    get_rows,
    get_cols,
    matrix_scan_custom
};

const bmp_matrix_func_t *get_matrix_func_user() { return &matrix_func; }
