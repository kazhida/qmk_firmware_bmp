//
// Created by 樋田一幸 on 2020/06/27.
//

#include "expander.h"
#include <i2c.h>
#include "../i2c_master.h"

#include "debug.h"

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
