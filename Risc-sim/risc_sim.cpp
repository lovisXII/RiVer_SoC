#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include "risc_sim.h"

using namespace std ;



ostream& operator<<(ostream& os, vector<char*> v)
{
    for(auto it = v.begin() ; it != v.end() ; ++it)
    {
        os << (*it) << endl ;
    }
    return os ;
}
ostream& operator<<(ostream& os, vector<string> v)
{
    for(auto it = v.begin() ; it != v.end() ; ++it)
    {
        os << (*it) << endl ;
    }
    return os ;
}

vector<string> extract_file(char* file_name)
{
    vector<string> instructions ;
    string instruction_string ;

    char instruction[100] ;
    fstream file (file_name) ;
    
    if(file)
    {
        cerr <<file_name << " is successfully open" << endl ;
        while(!file.eof())
        {
            file.getline(instruction,100) ;
            instruction_string = static_cast<string> (instruction) ;
            instructions.push_back(instruction) ;
        }
        cout << instructions << endl ;
        return instructions ;
    }
    else
    {
        cerr <<file_name << " wasn't open successfully" << endl ;
        exit(1) ;
    }
}
int convert_inst_to_binary (string s)
{
    if()
}




