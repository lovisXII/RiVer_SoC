#include <iostream>
#include <stdint.h>
#include <cstdio>
#include <cstring>

/* 
L'objectif est de voir comment a-b se comporte 
Cela permettra de bien setup la condition a<b ou a>b
*/

using namespace std ;


void convertion_to_binary(int a, int tableau[])
{
    int rest = 0 ;
    int quotient = 10 ;
    int compteur = 0 ;
    
    while(quotient != 0)
    {
        rest = a % 2;
        quotient = a / 2 ;
        a = quotient ;
        tableau[31-compteur] = rest ;
        compteur++ ;
    }
}

void add_one(int tableau[33])
{
    bool ok = true ;
    int i = 0 ;
    while(ok)
    {
        if(tableau[33-i] == 0)
        {
            tableau[33-i] = 1 ;
            ok = false ;
        }
        else
        {
            tableau[33-i] = 0 ;
        }
        i++ ;
    }


}

ostream& operator<<(ostream& os, int tab[])
{
    for(int i = 0 ; i < 33 ; i++)
    {
        os << tab[i] ;
    }
    return os ;
}

void ca2_of_32_bits(int a, int tableau[])
{
    bool neg = false ;
    if(a < 0 )
    {
        a = -a ;
        neg = true ;
    }
    convertion_to_binary(a, tableau) ;
    for(int i = 0 ; i < 32 ; i++)
    {
        if(tableau[i] == 1)
        {
            tableau[i] = 0 ;
        }
        else
        {
            tableau[i] = 1 ;
        }
    }
    add_one(tableau) ;
    cout << "Complement2 of " << a << " is : " << tableau << endl ;

}

int main(int argc, char* argv[])
{
    //Objective is to to a - b ;

    int a = atoi( argv[1]) ;
    int b = atoi(argv[2]) ;
    if(a < b)
    {
        cout << a << "< " << b << endl ;
        cout << "blt is taken which means that inc_pc = 0" << endl ;
        cout << "bge is not taken which means that inc_pc = 1" << endl ;
    }
    else
    {
        cout << a << ">= " << b << endl ;
        cout << "blt is not taken which means that inc_pc = 1" << endl ;
        cout << "bge is taken which means that inc_pc = 0" << endl ;

    }
    int c = a- b ;
    int tab_a[33] = {0};
    int tab_b[33] = {0};
    int tab_c[33] = {0};
    ca2_of_32_bits(c, tab_c) ;
    return 0 ;

}
