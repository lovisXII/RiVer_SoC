__asm__(".section .init");
__asm__(".global main");
__asm__("j main");

#include "driver_wb.h"
#include "driver_leds.h"

int main(void) {
    int i;
    for(i=0; i<5; i++) {
        leds_turn_on();
        wb_wait(50);
        leds_turn_off();
        wb_wait(50);
    }
    wb_write_reg(LEDS_ADR_REG1, 0x5);
    wb_write_reg(LEDS_ADR_REG2, 0x4);
    leds_train(20, 5);
    river_end(); 
        
    return 0;
}