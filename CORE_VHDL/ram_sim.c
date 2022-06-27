#pragma GCC push_options
#pragma GCC optimize ("O0")
#include <stdio.h>
#include <stdlib.h>
#include "elfio.h"

int start_pc;
int *instruction;
int *adr_instr; 
int NB_INSTR; 

int good_adr = 0; 
int bad_adr = 0; 

int*** ram[256];

int read_mem(int a) {
    int addr1, addr2, addr3, addr4;
    int adr = a;
    a = a >> 2; 
    addr1 = a & 0xFF; 
    addr2 = (a >> 8) & 0xFF; 
    addr3 = (a >> 16) & 0xFF; 
    addr4 = (a >> 24) & 0xFF; 
    if(ram[addr1] && ram[addr1][addr2] && ram[addr1][addr2][addr3]) {
        return ram[addr1][addr2][addr3][addr4];
    }
    return 0; 
}

int write_mem(int a, int data, int byt_sel) {
    int addr1, addr2, addr3, addr4;
    int tmp = 0; 
    int mask = 0;
    int adr = a;  
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

int get_good(int z) {
    return good_adr; 
}

int get_bad(int z) {
    return bad_adr; 
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
    printf("inside c file\n");
    if(strcmp(type_of_file,".c") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld %s",
                path);
        system((char*)temp);
        strcpy(output,"a.out") ;
    }  
    if(strcmp(type_of_file,".s") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld %s",
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
      
    printf("Number of Instruction : %d\n", (structure->size)/4) ;

    printf("Start Adress : %x\n",structure->start_adr) ;
    start_pc = (structure->start_adr);
    good_adr = mem_goodadr();
    bad_adr  = mem_badadr();

    int i = 0;
    int j = 0 ;

    int *instruction    = malloc(((structure->size)/4)*sizeof(int)) ;
    int *adresses       = malloc(((structure->size)/4)*sizeof(int)) ;

    printf("******LOADING INSTRUCTION*****\n");
    // Sections loading
    Elf32_Obj *pObj = structure->pObj_struct;

    for (int i=0; i< pObj->Head.e_shnum; i++)
    {
        for(int j = 0 ; j < (pObj->size[i]); j+=4){     
            printf("%8x : %8x\n",(pObj->Section_Hdr[i]->sh_addr)+j, mem_lw(pObj->Section_Hdr[i]->sh_addr+j)) ;
            write_mem((pObj->Section_Hdr[i]->sh_addr)+j,mem_lw(pObj->Section_Hdr[i]->sh_addr+j), 15);
        }
    }
    Del_Elf32(structure->pObj_struct);

    ghdl_main(argc - nargs, &argv[nargs]);
    return 0 ;
}

