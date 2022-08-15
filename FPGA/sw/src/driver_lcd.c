#include "driver_lcd.h"

void lcd_write(char* str, int size) {
    int i;
    for(i = 0; i < 80; i++) {
        if(i < size)
            wb_write_reg(LCD_ADR_REG0, (int)str[i]);
        else 
            wb_write_reg(LCD_ADR_REG0, 0x20);
    }
}

void lcd_hello() {
    int i;
    char* hello = "hello world from RiVer!";
    lcd_write(hello, 23);
}
