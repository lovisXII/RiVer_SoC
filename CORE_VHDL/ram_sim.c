#include <stdio.h>
#include <stdlib.h>
# include "elfio.h"

int start_pc;
int *instruction;
int *adr_instr; 
int NB_INSTR; 

int*** ram[256];

int get_mem(int a) {
    int addr1, addr2, addr3, addr4;
    int copy_adr_for_print = a;
    a = a >> 2; 
    addr1 = a & 0xFF; 
    addr2 = (a >> 8) & 0xFF; 
    addr3 = (a >> 16) & 0xFF; 
    addr4 = (a >> 24) & 0xFF; 
    if(ram[addr1] && ram[addr1][addr2] && ram[addr1][addr2][addr3]) {
        printf("adresse asked : %x, result it %x\n", copy_adr_for_print, ram[addr1][addr2][addr3][addr4]);
        return ram[addr1][addr2][addr3][addr4];
    }
    return 0; 
}

int write_mem(int a, int data, int byt_sel) {
    int addr1, addr2, addr3, addr4;
    int tmp = 0; 
    int mask = 0; 
    a = a >> 2; 
    addr1 = a & 0xFF; 
    addr2 = (a >> 8) & 0xFF; 
    addr3 = (a >> 16) & 0xFF; 
    addr4 = (a >> 24) & 0xFF; 
    if (!ram[addr1]) ram[addr1] = calloc(256, sizeof(int*));
    if (!ram[addr1][addr2]) ram[addr1][addr2] = calloc(256, sizeof(int*));
    if (!ram[addr1][addr2][addr3]) ram[addr1][addr2][addr3] = calloc(256, sizeof(int));
    if(byt_sel & 0x1) 
        mask |= 0xFF; 
    if(byt_sel & 0x2)
        mask |= 0xFF00;
    if(byt_sel & 0x4)
        mask |= 0xFF0000;
    if(byt_sel & 0x8)
        mask |= 0xFF000000; 
    tmp = ram[addr1][addr2][addr3][addr4];
    tmp &= ~mask; 
    tmp |= data & mask; 
    ram[addr1][addr2][addr3][addr4] = tmp;
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

    // Receiving arguments
    
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

    // Generation of executable file

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

    // Reading elf file, parsing it and getting sections and segments

    FILE_READ* structure = (FILE_READ*)malloc(sizeof(FILE_READ));
    structure = Read_Elf32(output);
      
    printf("Number of Instruction : %x\n", (structure->size)/4) ;

    printf("Start Adress : %x\n",structure->start_adr) ;
    start_pc = (structure->start_adr);
    int i = 0;
    int j = 0 ;

    int *instruction    = malloc(((structure->size)/4)*sizeof(int)) ;
    int *adresses       = malloc(((structure->size)/4)*sizeof(int)) ;

    printf("******LOADING INSTRUCTION*****\n");
    while(i < structure->size){
        printf("%x : %x\n",structure->start_adr+i , mem_lw(structure->start_adr+i)) ;
        write_mem(structure->start_adr+i,mem_lw(structure->start_adr+i), 15);
        i+=4 ;
        j++;
    }
    Del_Elf32(structure->pObj_struct);

    ghdl_main(argc - nargs, &argv[nargs]);
    return 0 ;
}

