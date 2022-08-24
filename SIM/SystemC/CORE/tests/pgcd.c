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
    int nbr1, nbr2 ;
    int pgcd, i;
	
	nbr1 = 5121;
    nbr2 = 891;
    
    for(i=1; i <= nbr1 && i <= nbr2; ++i)
    {
        if(nbr1%i == 0 && nbr2%i == 0)
            pgcd = i;
    }
	if(pgcd == 9)
    {
        _good() ;
    }
    else{
        _bad() ;
    }
}