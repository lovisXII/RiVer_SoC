extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");


#define val -6;
#define res -216

int main() {
    int b = val;

    b = b*b*b;

    if (b == res) {
        _good();
    }
    _bad();
}