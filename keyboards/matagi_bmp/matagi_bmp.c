#include "matagi_bmp.h"

#include QMK_KEYBOARD_H
#include "matrix.h"
#include "bmp.h"
#include "bmp_matrix.h"
#include "i2c.h"
#include <string.h>
#include "debug.h"
#include "report.h"

#define IO_NUM 5

static bool   ioexpander_init_succeed[IO_NUM] = {false};

void keyboard_post_init_user() {
    debug_enable = true;
}

void create_user_file() {
    static const char qmk_configurator[] = "<meta http-equiv=\"refresh\" content=\"0;URL=\'https://sekigon-gonnoc.github.io/qmk_configurator\'\"/>";
    BMPAPI->usb.create_file("MAP_EDITHTM", (uint8_t*)qmk_configurator, strlen(qmk_configurator));
}

static void create_status_file() {
    static char status_str[128];
    // clang-format off
    tfp_sprintf(status_str,
                "IO exp0:%s\r\n"
                "IO exp1:%s\r\n"
                "IO exp2:%s\r\n"
                "IO exp3:%s\r\n"
                "IO exp4:%s\r\n",
                ioexpander_init_succeed[0] ? "OK" : "NG",
                ioexpander_init_succeed[1] ? "OK" : "NG",
                ioexpander_init_succeed[2] ? "OK" : "NG",
                ioexpander_init_succeed[3] ? "OK" : "NG",
                ioexpander_init_succeed[4] ? "OK" : "NG");
    // clang-format on
    BMPAPI->usb.create_file("STATUS  TXT", (uint8_t *)status_str, strlen(status_str));
}

void matrix_init_kb() {
    // reset io expanders
//    setPinOutput(IO_RESET);
//    setPinOd(IO_ROW);
//
//    writePinLow(IO_RESET);
//
//    writePinHigh(CS_PIN_IO);
//    writePinHigh(CS_PIN_TB);
//    setPinOutput(CS_PIN_IO);
//    setPinOutput(CS_PIN_TB);
//
//    // turn off trackball
//    setPinOutput(TB_POW);
//    writePinHigh(TB_POW);
//
//    writePinHigh(IO_RESET);

    matrix_init_user();
}

void matrix_scan_kb() {
    create_status_file();

    if (!is_usb_connected()) {
        static uint32_t battery_check_cnt = 0;
        static float voltage = -1;
        static uint32_t low_battery_cnt = 0;

        if (voltage < 0) {
            // first measurement
            voltage = BMPAPI->app.get_vcc_mv();
            battery_check_cnt = timer_read32();
        } else if (timer_elapsed32(battery_check_cnt) > 1000) {
            battery_check_cnt = timer_read32();
            uint32_t v_mes =  BMPAPI->app.get_vcc_mv();

            if (v_mes > voltage) {
                voltage = (0.7f) * voltage + (0.3f) * v_mes;
            } else {
                if (voltage - v_mes > 100) {
                } else {
                    voltage = (0.95f) * voltage + (0.05f) * v_mes;
                }
            }

            if (voltage < 1000.0f - 100.0f) { // 100mV offset by series resistance
                low_battery_cnt++;
            }

            if (low_battery_cnt > 30) {
                sleep_enter_counter = 1;
            }
        }
    }

    matrix_scan_user();
}

void bmp_before_sleep() {
//    setPinInputHigh(16);
//    setPinInputHigh(14);
//    setPinInputHigh(CS_PIN_TB);
//    writePinHigh(TB_POW);
}

bool checkSafemodeFlag(bmp_api_config_t const *const config) { return false; }

bool bmp_config_overwrite(bmp_api_config_t const *const config_on_storage,
                          bmp_api_config_t *const       keyboard_config) {
    // get vcc from a3
    BMPAPI->adc.config_vcc_channel(3, 1200, 700);
    // wait until voltage become stable
    for (int i = 0; i < 40000; i++) {
        BMPAPI->app.get_vcc_percent();
    }

    // User can overwrite partial settings
    keyboard_config->startup          = config_on_storage->startup;
    keyboard_config->matrix.debounce  = config_on_storage->matrix.debounce;
    keyboard_config->param_central    = config_on_storage->param_central;
    keyboard_config->param_peripheral = config_on_storage->param_peripheral;
    keyboard_config->keymap           = config_on_storage->keymap;
    keyboard_config->reserved[2]      = config_on_storage->reserved[2];
    return true;
}

static uint8_t initialized = 0;

#define TIMEOUT 100

#define ALL_OUTPUT 0
#define ALL_INPUT 0xFF
#define ALL_LOW 0
#define ALL_HIGH 0xFF

void expander_init(const expander *slaves, uint8_t count) {

    if (!initialized) {
        initialized++;
        i2c_init();
    }

    // make all pins to input mode
    for (uint8_t i = 0; i < count; i++) {
        uint8_t num = slaves[i].num_port;
        uint8_t addr = slaves[i].address << 1;
        ioexpander_init_succeed[i] = 1;
        for (uint8_t j = 0; j < num; j++) {
            uint8_t cmd = CMD_CONFIG * num + j;
            uint8_t conf = ALL_INPUT;
            uint8_t ret = i2c_writeReg(addr, cmd, &conf, sizeof(conf), TIMEOUT);
            if (ret != 0) {
                xprintf("config pins FAILED: %d, addr: %02X, cmd: %02X conf: %d\n", ret, addr, cmd, conf);
            } else {
                xprintf("config pins SUCCEEDED: %d, addr: %02X, cmd: %02X conf: %d\n", ret, addr, cmd, conf);
            }
            ioexpander_init_succeed[i] &= ret == 0;
        }
    }
}

uint16_t expander_readPins(const expander *slave) {
    uint8_t addr = slave->address << 1;
    uint8_t data[2] = { 0xff, 0xff };     // num_port <= 2
    uint8_t cmd = CMD_INPUT * slave->num_port;

    uint8_t ret = i2c_readReg(addr, cmd, data, slave->num_port, TIMEOUT);
    if (ret != 0) {
//        xprintf("i2c_readReg FAILED: %d, addr: %02X, cmd: %02X\n", ret, addr, cmd);
        return 0x0000;
    } else {
//        xprintf("i2c_readReg SUCCEED: %d, addr: %02X, cmd: %02X\n", ret, addr, cmd);
    }
    uint16_t result = data[1] << 8 | data[0];

    return (~result) & slave->mask;
}

static expander expanders[MATRIX_ROWS] = {
    PCA9555(0x01),
    PCA9555(0x02),
    PCA9555(0x03),
    PCA9555(0x04),
    PCA9555(0x05)
};

static void mtx_init() {
    debug_enable = false;
    expander_init(expanders, MATRIX_ROWS);
}

static uint32_t mtx_get_row() { return MATRIX_ROWS_DEFAULT; }

static uint32_t mtx_get_col() { return MATRIX_COLS_DEFAULT; }

static uint32_t mtx_scan(matrix_row_t *current_matrix) {
    bool changed = false;

//    dprint("matrix_scan_custom");
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix_row_t last_row = current_matrix[i];
        matrix_row_t current_row = expander_readPins(&expanders[i]);
//        if (current_row != 0 && current_row != last_row) {
//            xprintf("row[%d]: %04X -> %04X\n", i, last_row, current_row);
//        }
        current_matrix[i] = current_row;
        changed |= (last_row != current_row);
    }

    return changed;
}

static const bmp_matrix_func_t matrix_func = {
    mtx_init,
    mtx_get_row,
    mtx_get_col,
    mtx_scan
};

const bmp_matrix_func_t *get_matrix_func_user() { return &matrix_func; }
