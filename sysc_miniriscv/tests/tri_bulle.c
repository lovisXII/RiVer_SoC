extern void _bad();
extern void _good();

//needed to copy an array
void* memcpy(void* dest, const void* source, unsigned int size) 
{
    char* char_source = (char*) source;
    char* char_dest = (char*) dest;
    for (int i = 0; i < size; i++) 
        char_dest[i] = char_source[i]; 
    return dest;
}

int main() {
    int tab[20] = {2,8,4,5,6,8,7,1,0,5,0,4,7,8,9,9,6,3,4,5};
    
    for(int i = 19; i >= 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(tab[j] > tab[j+1])
            {
                int tmp = tab[j];
                tab[j] = tab[j+1];
                tab[j+1] = tmp;
            }
        }
    }
    int tri = 1;
    for(int i = 0; i < 19; i++)
    {
        if(tab[i] > tab[i+1])
        {
            tri = 0;
        }
    }
    if(tri)
        _good();
    _bad();
}