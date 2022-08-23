#include "driver_wb.h"

int wb_read_reg(int adr) {
    return *(int*)adr;
}

void wb_write_reg(int adr, int data) {
    int* tmp = (int*)adr;
    *tmp = data;
}

void wb_wait(int delay) {  
    int i; 
    int tmp = (FREQ_RIVER*delay)/(1000);

    for(i = 0; i < tmp; ++i) {
        __asm__("nop");
    }
}

void river_end() {
    __asm__("deadloop: nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("j deadloop");
}