#include "driver_leds.h"


void leds_turn_on() {
    wb_write_reg(LEDS_ADR_REG0, 0xFFFF);
    wb_write_reg(LEDS_ADR_REG1, 0x7);
    wb_write_reg(LEDS_ADR_REG2, 0x7);
}

void leds_turn_off() {
    wb_write_reg(LEDS_ADR_REG0, 0);
    wb_write_reg(LEDS_ADR_REG1, 0);
    wb_write_reg(LEDS_ADR_REG2, 0);
}

void leds_train(int delay, int tour) {
    int i, j;
    int tours = 2*tour;
    int led_state = 0x1;
    int direction = 1;  // 0 = right, 1 = left
    for(i = 0; i < tours; i++) {
        for(j = 0; j <= 15; j++) {
            if(direction)
                led_state <<= 1;
            else 
                led_state >>= 1;
            wb_write_reg(LEDS_ADR_REG0, led_state);
            wb_wait(delay);
        }
        direction = direction ? 0:1; 
    }
}