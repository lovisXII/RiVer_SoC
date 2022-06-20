extern void _bad();
extern void _good();

__asm__(".section .text") ;
__asm__(".global _start") ;


__asm__("_start:");
__asm__("addi x1,x1, 4");
__asm__("sub x2, x2,x1 ");
__asm__("jal x5, main");



int det(int mat[4][4], int n, int* index)
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
        D += sign*mat[0][f]*det(temp, n - 1, index);
        sign = -sign;
    }
    
    switch(*index)
    {
        case 0:
            if(D != 15)
                _bad();
        break;
        case 1:
            if(D != 0)
                _bad();
        break;
        case 2:
            if(D != 5)
                _bad();
        break;
        case 3:
            if(D != 25)
                _bad();
        break;
        case 4:
            if(D != 15)
                _bad();
        break;
        case 5:
            if(D != 3)
                _bad();
        break;
        case 6:
            if(D != 6)
                _bad();
        break;
        case 7:
            if(D != 75)
                _bad();
        break;
        case 8:
            if(D != 0)
                _bad();
        break;
        case 9:
            if(D != 3)
                _bad();
        break;
        case 10:
            if(D != -1)
                _bad();
        break;
        case 11:
            if(D != -5)
                _bad();
        break;
        case 12:
            if(D != 5)
                _bad();
        break;
        case 13:
            if(D != 6)
                _bad();
        break;
        case 14:
            if(D != -1)
                _bad();
        break;
        case 15:
            if(D != 15)
                _bad();
        break;
        case 16:
            if(D != 30)
                _bad();
        break;
    }
    (*index)++;
    return D;
}

int main() {
    _good();
    int mat[4][4] = { { 1, 0, 2, -1 },
                      { 3, 0, 0, 5 },
                      { 2, 1, 4, -3 },
                      { 1, 0, 5, 0 } };

    int index = 0;
    int b = det(mat, 4, &index);

    if (b == 30) {
        _good();
    }
    _bad();
}
