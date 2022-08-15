#include "driver_wb.h"

#define LEDS_BASE_ADR 0x40000000

#define LEDS_ADR_REG0 LEDS_BASE_ADR
#define LEDS_ADR_REG1 (LEDS_BASE_ADR + 4)
#define LEDS_ADR_REG2 (LEDS_BASE_ADR + 8)
#define LEDS_ADR_REG3 (LEDS_BASE_ADR + 12)

void    leds_turn_on    ();
void    leds_turn_off   ();
void    leds_train      (int delay, int tour);