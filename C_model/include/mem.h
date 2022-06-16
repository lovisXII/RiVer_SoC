#ifndef MEM
#define MEM

#include <stdint.h>

extern uint32_t GoodAdr;
extern uint32_t BadAdr;

uint32_t mem_goodadr();
uint32_t mem_badadr();

int mem_add_seg(uint32_t adr, size_t size, void *src);

uint32_t mem_lw(uint32_t adr);
int mem_sw(uint32_t adr, uint32_t data);

uint8_t mem_lb(uint32_t adr);
int mem_sb(uint32_t adr, uint8_t data);

void init_mem(char *Elf_file);

void mem_free();
#endif
