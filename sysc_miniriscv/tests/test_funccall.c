extern void _bad();
extern void _good();

int somme(int* a)
{
    return (*a)++ ;
}
int main() {
    int a = 3;
    int b = 4;
    for (;a > 0; a--) {
        somme(&b);
    }

    if (b == 7) {
        _good();
    }
    _bad();
}