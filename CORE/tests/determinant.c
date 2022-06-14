extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");


void * memcpy( void * destination, const void * source, unsigned int size ) {
   char* char_source = (char*) source;
   char* char_dest = (char*) destination;
   for (int i = 0; i < size; i++) char_dest[i] = char_source[i]; 
   return destination;
}

int det(int mat[4][4], int n)
{
    int D = 0; 
    if(n == 1)
        return mat[0][0];

    int temp[4][4];
    int sign = 1; 

    for (int f = 0; f < n; f++)
    {
        int i = 0, j = 0;
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (row != 0 && col != f)
                {
                    temp[i][j++] = mat[row][col];
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
        D += sign*mat[0][f]*det(temp, n - 1);
        sign = -sign;
    }
 
    return D;
}

int main() {
    int mat[4][4] = { { 1, 0, 2, -1 },
                      { 3, 0, 0, 5 },
                      { 2, 1, 4, -3 },
                      { 1, 0, 5, 0 } };

    int b = det(mat, 4);

    if (b == 30) {
        _good();
    }
    _bad();
}
