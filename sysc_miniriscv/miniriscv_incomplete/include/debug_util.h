#pragma once

#include <string>
/* La macro GET_NAME permet de générer le nom d'un signal de la forme core.alu_inst1.alu_cmd par
exemple cela permet d'avoir un fichier .vcd hiérachisé, de manière plus ou moins automatisée */

// This macro calls a function, with the name of the signal according to system
// (something like exec.alu.port0) and replaces it with exec.alu.CIN. In
// practice, it replaces everything after the last dot with the name of the
// variable given in argument
//(#signal means "the name of signal")
#define GET_NAME(signal) signal_get_name(signal.name(), #signal)

inline std::string signal_get_name(const char* sysc_name, const char* given_name) {
    std::string sysc(sysc_name);
    std::string cropped = sysc.substr(0, sysc.find_last_of('.') + 1);
    cropped += given_name;
    return cropped;
}