#define FREQ_RIVER  45000000    // Hz  

int wb_read_reg(int adr);
void wb_write_reg(int adr, int data);

void wb_wait(int delay);        // ms
void river_end();