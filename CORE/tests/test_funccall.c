extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x2,x0, 0x100");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x1,x2 ");
__asm__("jal x5, main");


int somme(int* a)
{
    return (*a)++ ;
}
int main() {
    int a = 3;
    int b = 4;
    for (;a > 0; a--) {
        somme(&b);
    }

    if (b == 7) {
        _good();
    }
    _bad();
}

__asm__("nop");
__asm__("_bad:");
__asm__("    add x0, x0, x0");
__asm__("_good :");
__asm__("    add x1, x1, x1");