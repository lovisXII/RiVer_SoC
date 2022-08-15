__asm__(".section .init");
__asm__(".global main");

#include "driver_wb.h"
#include "driver_leds.h"
#include "driver_lcd.h"

int main(void) {
    int i = 0;
    while(i < 10) {
        leds_turn_on();
        wb_wait(200);
        leds_turn_off();
        wb_wait(200);
        i++;
    }
    return 0;
}