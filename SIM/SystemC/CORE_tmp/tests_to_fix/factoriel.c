extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");

<<<<<<< HEAD
#define val 10
#define res 3628800
int fact(int a)
=======
#define val 6//20
#define res -720//2432902008176640000
long long fact(int a)
>>>>>>> ac6affb0 (more test still not working signed multiplication)
{
    long long b = 1;
    for(int i = 2; i <= a; i++)
    {
        b = b*-i;
    }
    return b;
}
int main() {
    long long b = fact(val);

    if (b == (long long)res) {
        _good();
    }
    _bad();
}