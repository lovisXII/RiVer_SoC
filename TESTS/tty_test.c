extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");

int main()
{
    volatile int *getc = (int*)0X05000000;
    volatile int *isch = (int*)0X05000004;
    *isch = 0;
    int TTY_BASE_ADR = 0x05000010;
    volatile char * buf = (char*) TTY_BASE_ADR;
    buf[0] = '>';
    int i = 1;
    while(1) {
        if (*isch) {
            *isch = 0;
            buf[i++] = *getc;
        }
    }
}