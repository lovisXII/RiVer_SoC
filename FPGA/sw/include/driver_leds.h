#include "driver_wb.h"

#define LEDS_BASE_ADR 0x40000000

#define LEDS_ADR_REG0 LEDS_BASE_ADR         // slave register 0 : 16 lsb used for 16 leds
#define LEDS_ADR_REG1 (LEDS_BASE_ADR+0x4)   // slave register 1 : 3  lsb used (rgb0) : bit0 = red, bit1 = green, bit2 = blue
#define LEDS_ADR_REG2 (LEDS_BASE_ADR+0x8)   // slave register 1 : 3  lsb used (rgb1) : bit0 = red, bit1 = green, bit2 = blue
#define LEDS_ADR_REG3 (LEDS_BASE_ADR+0xC)   // unused

void    leds_turn_on    ();                         // turn on all leds 
void    leds_turn_off   ();                         // turn off all leds
void    leds_train      (int delay, int tour);