#include <iostream>

using namespace std ;

int main(int argc, char* argv[])
{
    int a = 0x6B8B4567 ;
    unsigned int b ;

    b = ((unsigned int)a) >> (0x327B23C6 && 0x1F) ;
    cout << b << endl ;
    cout << 0x00D7168A << endl ;
    cout << 0x00D7168A << endl ;
}