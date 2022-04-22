extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("lui x2,0x8100") ;
__asm__("addi x2,x0, 0x100");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x1,x2 ");
__asm__("jal x5, main");

int main()
{
    int a, i;
    a = 1;
    for(i=1; i <= 1; ++i)
    {
            a = i;
    }
	if(a == 1)
    {
        _good() ;
    }
    else{
        _bad() ;
    }
}