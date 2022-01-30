#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std ;



    //enum instruction_type {R_type, I_type,S_type, B_type, U_type, J_type} ;
    ostream&            operator<<                  (ostream& os, vector<char*> v);
    ostream&            operator<<                  (ostream& os, vector<string> v);
    vector<string>      extract_file                (char* file_name);
    int                 convert_inst_to_binary      (string) ;


    