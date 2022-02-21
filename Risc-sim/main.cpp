#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include "risc_sim.h"

using namespace std ;

int main(int argc, char* argv[])
{
   vector<string> s;
   string test ;
   s = extract_file(argv[1]) ;
   string lala = convert_inst_to_binary(s[0]) ; 
}