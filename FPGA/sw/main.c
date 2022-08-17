__asm__(".section .init");
__asm__(".global main");
__asm__("j main");

#include "driver_wb.h"
#include "driver_leds.h"
#include "driver_lcd.h"

int main(void) {
    __asm__("li x27, 0xF");
    __asm__("li x28, 0x40000000");
    __asm__("sw x27, 0(x28)");
    return 0;
}