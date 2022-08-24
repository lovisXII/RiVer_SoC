extern void _bad();
extern void _good();

int modulo(int a, int b){
    while(a >= b) a -= b;
    return a ;
}
int main()
{
    int nbr1, nbr2 ;
    int pgcd, i;
	
	nbr1 = 3 ;
    nbr2 = 7 ;
    
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