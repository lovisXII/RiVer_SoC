extern void _bad();
extern void _good();

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