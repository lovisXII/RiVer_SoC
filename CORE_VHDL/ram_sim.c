#include <stdio.h>
#include <stdlib.h>

#define MAX_INST 1000

#define filename "../test.txt"

int instr[MAX_INST];
int ram[1000];
int cur_inst = 0; 

int get_inst(int a) {
    a = a >> 2;
    return ram[a];
}

int get_mem(int a) {
    a = a >> 2; 
    return ram[a];    
}

int write_mem(int a, int data) {
    a = a >> 2;
    ram[a] = data; 
    return 0;
}

extern int ghdl_main(int argc, char const* argv[]);


int main(int argc, char const* argv[]) {
    char* line_buf = NULL; 
    size_t line_buf_size = 0; 
    FILE* file; 
    file = fopen(filename, "r");

    if(file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return EXIT_FAILURE;
    }

    while(getline(&line_buf, &line_buf_size, file) > 0) {
        printf("%s", line_buf);
        int i = strtol(line_buf, NULL, 16);
        write_mem(4*cur_inst, i);
        instr[cur_inst++] = i;
    }
    ghdl_main(argc, argv);
}

