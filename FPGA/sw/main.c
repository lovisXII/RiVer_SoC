__asm__(".section .init");
__asm__(".global main");
__asm__("j main");

#include "driver_wb.h"
#include "driver_leds.h"
#include "driver_lcd.h"

int main(void) {
    int i;
    for(i=0; i<5; i++) {
        leds_turn_on();
        wb_wait(100);
        leds_turn_off();
        wb_wait(100);
    }
    return 0;
}