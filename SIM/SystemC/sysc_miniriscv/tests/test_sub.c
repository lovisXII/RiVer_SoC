extern void _bad();
extern void _good();


int main()
{
    int nbr1, nbr2 ;
	
	nbr1 = 1 ;
    nbr2 = 10 ;
    
    nbr2 -= nbr1;
    if (nbr2 == 9) {
        _good();
    }
    else {
        _bad();
    }
}