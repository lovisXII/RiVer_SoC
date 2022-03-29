#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include "risc_sim.h"
#include "instruction.h"


using namespace std ;

status cur_state = IDLE;
instruction_type cur_inst = None;


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

ostream& operator<<(ostream& os, instruction_type inst)
{
    switch(inst)
    {
        case 0 :
            os << "R_type" << endl ;
        case 1 :
            os << "I_type" << endl ;
        case 2 :
            os << "S_type" << endl ;
        case 3 :
            os << "B_type" << endl ;
        case 4 :
            os << "U_type" << endl ;
        case 5 :
            os << "J_type" << endl ;
        case 6 :
            os << "JARL_type" << endl ;
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

bool checking_r_type( string s)
{
    bool is_type = 0 ;

    string instruction = parsor(s, ", ") ;

    for(size_t i = 0 ; i < r_type_inst.size() ; i++)
    {
        if(instruction == r_type_inst[i])
        {
            is_type = 1 ;
            break ;
        }
    }
    return is_type ;
}

string parsor (string &s, string tab_caractere)
{
    size_t i ;
    string string_parsed ;
    cout << " s: " << s << endl ;
    for(i = 0 ; i < s.size() ; i++)
    {
        if(s[i] == tab_caractere[0] || s[i] == tab_caractere[1]) 
        {
            cout << "inside if " << i << endl ;
            break ;
        }
    }
    string_parsed = s.substr(0, i) ;
    s = s.substr(i+1, s.size()) ;
    cout << "string parsed :" << string_parsed ;
    return string_parsed ;
    
}


void fsm_instruction(string s)
{
    if(checking_r_type(s))
    {
        cur_inst = R_type ;
    }

}

string convert_inst_to_binary(string instruction)//must receive r[0]
{
    string funct7 ;
    string rs2 ;
    string rs1 ;
    string funct3;
    string rd ;
    string opcode ;
    fsm_instruction(instruction) ;
    switch(cur_inst)
    {

        case R_type :
            string instruction_name = parsor(instruction, ", ") ; // going to get name instruction
            funct7 = funct7_r_type[instruction_name] ;
            funct3 = funct3_r_type[instruction_name] ;
            string rs1_text = parsor(instruction,", ") ;
            rs1_text = parsor(rs1_text, "r") ;
            return "lal" ;
    }
}

void fsm_state(status cur_state, char* file_name)
{
    vector<string> instructions = extract_file(file_name) ;
    cur_state = RUN ;
    while(cur_state != END_FILE)
    {
        
    }
}



