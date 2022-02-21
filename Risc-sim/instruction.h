#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>

static const vector<string> r_type_inst = {
    "add",
    "slt",
    "sltu",
    "and",
    "or",
    "xor",
    "sll",
    "srl",
    "sub",
    "sra"
};

std::map <string, string>  funct3_r_type
{
    {"add" ,"000"},
    {"slt" ,"010"},
    {"sltu","011"},
    {"and" ,"111"},
    {"or"  ,"110"},
    {"xor" ,"100"},
    {"sll" ,"001"},
    {"srl" ,"101"},
    {"sub" ,"000"},
    {"sra" ,"101"}
};

std::map <string, string>  funct7_r_type
{
    {"add"  ,"0000000"},
    {"slt"  ,"0000000"},
    {"sltu" ,"0000000"},
    {"and"  ,"0000000"},
    {"or"   ,"0000000"},
    {"xor"  ,"0000000"},
    {"sll"  ,"0000000"},
    {"srl"  ,"0000000"},
    {"sub"  ,"0100000"},
    {"sra"  ,"0100000"}
};