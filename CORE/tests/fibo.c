extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;

__asm__("_start:");
__asm__("addi x2,x0, 0x100");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x1,x2 ");
__asm__("jal x5, main");

int fib(int n) {
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return 1;
    }
    else {
        return fib(n-1) + fib(n-2);
    }
}

int main() {
    if (fib(10) == 55) {
        _good();
    }
    else {
        _bad();
    }
}

__asm__("nop");
__asm__("_bad:");
__asm__("    add x0, x0, x0");
__asm__("_good :");
__asm__("    add x1, x1, x1");