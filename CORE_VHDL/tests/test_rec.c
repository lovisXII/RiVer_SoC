extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");
__asm__("nop");
__asm__("nop");

void nothing() {
    __asm__("nop");
    __asm__("nop");
}

void _bad() {
    __asm__("nop");
    __asm__("nop");
    __asm__("addi x20, x0, 1")
}
void _good() {
    __asm__("nop");
    __asm__("nop");
}

int somme(int a)
{
    if (a) return 1+somme(a-1);
    else return 0;
}

int main() {
    if (somme(0) == 0) {
        nothing();
        _good();    
    } else {
        nothing();
        _bad();
    }
    return 0;
}