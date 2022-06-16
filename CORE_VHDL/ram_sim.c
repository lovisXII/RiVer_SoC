#include <stdio.h>
#include <stdlib.h>
# include "elf.h"

#define MAX_INST 1000

int cur_inst = 0; 
int start_pc;
int *instruction;
int *adr_instr; 

int *ram; 
int *adr_ram;

int SIZE; 

int get_index(int a, int size) {
    for(int i = 0; i < size; i++) {
        if(a == adr_instr[i]) 
            return i; 
    }
    printf("index not found\n");
    return -1;
}

int get_inst(int a) {
    int i = get_index(a, SIZE);
    return instruction[i];
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

int get_startpc(int z) {
    return start_pc; 
}


extern int ghdl_main(int argc, char const* argv[]);


int main(int argc, char const* argv[]) {

    char   signature_name[20] ="";
    char   opt[20] = "";
    int    riscof = 0 ;
    char   path[30] ;
    char   output[30] ;
    char   test[512] = "> a.out.txt";
    int nargs = 1;

    strcpy(path,argv[1]) ;

    if (argc >= 3 && strcmp(argv[2],"-O") == 0) {
        nargs = 2;
        strcpy(opt,"-02") ;
    } else if (argc >= 4 && strcmp(argv[2],"--riscof") == 0) {
        nargs = 3;
        strcpy(signature_name,argv[3]);
        riscof         = 1;
    };

    char temp_text[512];
    char point = '.' ;
    char *type_of_file = strrchr(path,point) ; 

    if(strcmp(type_of_file,".c") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im %s",
                path);
        system((char*)temp);
        strcpy(output,"a.out") ;
    }  
    if(strcmp(type_of_file,".s") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im %s",
                path);
        system((char*)temp);
        strcpy(output,"a.out") ;
    }  

    sprintf(temp_text, "riscv32-unknown-elf-objdump -D %s", output);
    strcat(temp_text, test);
    system((char*)temp_text);

    Elf32_Obj *pObj = NULL;
    pObj = Read_Elf32(argv[1]);
    
    FILE_READ structure;

    structure = init_mem(output) ;
    printf("Number of Instruction : %x\n", (structure.size)/4) ;

    printf("Start Adress : %x\n",structure.start_adr) ;
    start_pc = structure.start_adr; 
    int i = 0;
    int j = 0 ;
    instruction = malloc(((structure.size)/4)*sizeof(int)) ;
    adr_instr = malloc(((structure.size)/4)*sizeof(int)) ;
    SIZE = structure.size;

    while(i < structure.size){
        instruction[j]  = mem_lw(structure.start_adr+i) ;
        adr_instr[j]     = structure.start_adr+i ;
        i+=4 ;
        j++;
    }
    for(int i = 0 ; i < (structure.size)/4 ; i++){
        write_mem(i,instruction[i]) ;
        printf("%8x : %8x\n", adr_instr[i], instruction[i]) ;
    }
    Del_Elf32(pObj);
    printf("end of main\n") ;


    // char* line_buf = NULL; 
    // size_t line_buf_size = 0; 
    // FILE* file; 
    // file = fopen(filename, "r");

    // if(file == NULL) {
    //     printf("Erreur lors de l'ouverture du fichier %s\n", filename);
    //     return EXIT_FAILURE;
    // }

    // while(getline(&line_buf, &line_buf_size, file) > 0) {
    //     printf("%s", line_buf);
    //     int i = strtol(line_buf, NULL, 16);
    //     write_mem(4*cur_inst, i);
    //     instr[cur_inst++] = i;
    printf("plop %d\n", nargs);
    ghdl_main(argc - nargs, &argv[nargs]);
}

