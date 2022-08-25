extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");

#define res 66699889
#define size 34

void* memcpy( void * destination, const void * source, unsigned int size ) {
   char* char_source = (char*) source;
   char* char_dest = (char*) destination;
   for (int i = 0; i < size; i++) char_dest[i] = char_source[i]; 
   return (void*)char_dest;
}

int func()
{
    int mat[10] = {5, 3, 87, -8, 35, -74, -12, 29, 64, -78, 
                -3, 45, 48, -26, -37, -103, 25, 44, -66, 
                -777, 8956, -1214, 125, -663, 55, -64,
                1256, -784, -999, 741, 123, 369, 987, 78};

    int calc = 0;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            calc += mat[i]*mat[j];
        }
    }
    return calc;
}

int main() {

    int c = func();

    if (c == res) {
        _good();
    }
    _bad();
}