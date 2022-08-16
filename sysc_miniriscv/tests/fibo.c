extern void _bad();
extern void _good();

int fib(int n) {
    if (n == 0) {
        return 0;
    }
    else if (n == 1) {
        return 1;
    }
    else {
        return fib(n-1) + fib(n-2);
    }
}



int main() {
    if (fib(10) == 55) {
        _good();
    }
    else {
        _bad();
    }
}