#include <stdio.h>

int modulo(int a, int b){
    int r ;
    while((a - b) >= 0 )
    {
        r = a-b ;
        a = r ;
    }
    return r ;
}
int main()
{
    int nbr1, nbr2 ;
    int pgcd, i;
	
	nbr1 = 1206554 ;
    nbr2 = 1634725 ;
    
    for(i=1; i <= nbr1 && i <= nbr2; ++i)
    {
        if(modulo(nbr1,i) == 0 && modulo(nbr2,i) == 0)
            pgcd = i;
    }
	if(pgcd == 1)
    {
        printf("ca marche ta daronne") ;
        return 0 ;
    }
    else{
        printf("ca marche poooo") ;
        return 1;
    }
}