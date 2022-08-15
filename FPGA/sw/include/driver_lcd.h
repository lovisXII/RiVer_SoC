#include "driver_wb.h"

#define LCD_BASE_ADR    0x60000000

#define LCD_ADR_REG0    LCD_BASE_ADR
#define LCD_ADR_REG1    (LCD_BASE_ADR + 4)
#define LCD_ADR_REG2    (LCD_BASE_ADR + 8)
#define LCD_ADR_REG3    (LCD_BASE_ADR + 12)

void    lcd_write(char* str, int size);
void    lcd_hello();
