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

int modulo(int a, int b){
    while(a >= b) a -= b;
    return a ;
}
int main()
{
    int nbr1, nbr2 ;
    int pgcd, i;
	
	nbr1 = 101 ;
    nbr2 = 54 ;
    
    for(i=1; i <= nbr1 && i <= nbr2; ++i)
    {
        if(modulo(nbr1,i) == 0 && modulo(nbr2,i) == 0)
            pgcd = i;
    }
	if(pgcd == 1)
    {
        _good() ;
    }
    else{
        _bad() ;
    }
}

__asm__("nop");
__asm__("_bad:");
__asm__("    add x0, x0, x0");
__asm__("_good :");
__asm__("    add x1, x1, x1");