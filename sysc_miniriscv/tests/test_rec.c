extern void _bad();
extern void _good();


int somme(int a)
{
    if (a) return 1+somme(a-1);
    else return 0;
}

int main() {
    if (somme(0) == 0) _good();
    _bad();
}