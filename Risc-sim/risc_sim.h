#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std ;

/*
    extract_file : take a .s file and extract each line, which are stored into a vector<string>
    
    fsm_instruction : allow the programm to determinate which time of instruction is inside extract_file()[i]
    
    fsm_state : tells the programm if all the file have been process
    
    parsor : get words separed by a coma

    checking_r_type : take a string and say if the string (which is an instruction) is an r_type one or no
*/

    enum status {IDLE, RUN, END_FILE} ;
    enum instruction_type {None, R_type, I_type,S_type, B_type, U_type, J_type, JARL_type} ;


    ostream&            operator<<                  (ostream& os, vector<char*> v);
    ostream&            operator<<                  (ostream& os, vector<string> v);
    
    vector<string>      extract_file                (char* file_name);
    void                fsm_instruction             (string);
    void                fsm_state                   (status cur_state, char*) ;
    string              parsor                      (string&, string) ;
    bool                checking_r_type             (string) ;
    void                affectation_r_type          ();
    
    string              convert_inst_to_binary      (string) ;


