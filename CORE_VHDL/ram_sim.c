#pragma GCC push_options
#pragma GCC optimize ("O0")
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "elfio.h"

int start_pc;
int *instruction;
int *adr_instr; 
int NB_INSTR; 

int good_adr = 0; 
int bad_adr = 0; 
int exception_adr = 0; 

int*** ram[256];

//riscof parameters
int riscof = 0 ;
int begin_signature = 0 ;
int end_signature = 0;
int signature_size = 0 ;
int rvtest_code_end = 0;
int **signature_value ;
FILE *riscof_signature ;

int read_mem(int a) {
    int addr1, addr2, addr3, addr4;
    int adr = a;
    a = a >> 2; 
    addr1 = a & 0xFF; 
    addr2 = (a >> 8) & 0xFF; 
    addr3 = (a >> 16) & 0xFF; 
    addr4 = (a >> 24) & 0xFF; 
    if(ram[addr1] && ram[addr1][addr2] && ram[addr1][addr2][addr3]) {
        printf("[read mem] : at @ %x data %x\n", adr, ram[addr1][addr2][addr3][addr4]);
        return ram[addr1][addr2][addr3][addr4];
    }
    return 0; 
}

int end_simulation(int result, int riscof_enable) {
    if(!riscof_enable)
        exit(result);
    else{
        if(begin_signature && end_signature)
        {    
            signature_size = (end_signature - begin_signature)/4 ;
            signature_value = calloc(signature_size*sizeof(int), signature_size*sizeof(int));
            for(int i = 0 ; i < signature_size ; i++)
            {
                signature_value[i] = read_mem(begin_signature+i*4) ;
                fprintf(riscof_signature,"%08x\n",signature_value[i]) ;
            }
        }
        exit(result);
    }
    
}

int write_mem(int a, int data, int byt_sel) {
    int addr1, addr2, addr3, addr4;
    int tmp = 0; 
    int mask = 0;
    int adr = a;  
    int dataw; 
    a = a >> 2; 
    addr1 = a & 0xFF; 
    addr2 = (a >> 8) & 0xFF; 
    addr3 = (a >> 16) & 0xFF; 
    addr4 = (a >> 24) & 0xFF; 
    if (!ram[addr1]) ram[addr1] = calloc(256, sizeof(int*));
    if (!ram[addr1][addr2]) ram[addr1][addr2] = calloc(256, sizeof(int*));
    if (!ram[addr1][addr2][addr3]) ram[addr1][addr2][addr3] = calloc(256, sizeof(int));

    switch(byt_sel) {
        // store byte
        case 1:     dataw = data            & ~(0xFFFFFF00);    break;
        case 2:     dataw = (data << 8)     & ~(0xFFFF00FF);    break;
        case 4:     dataw = (data << 16)    & ~(0xFF00FFFF);    break;
        case 8:     dataw = (data << 24)    & ~(0x00FFFFFF);    break;
        // store half word 
        case 3:     dataw = data            & ~(0xFFFF0000);    break;
        case 12:    dataw = (data << 16)    & ~(0x0000FFFF);    break;
        // store word
        case 15:    dataw = data;                               break;

        default:    dataw = 0;                                  break; 
    }

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
    tmp |= dataw; 
    ram[addr1][addr2][addr3][addr4] = tmp;
    printf("[write mem] : at @ %x writting %x, data w : %x, byt_sel = %d\n", adr, data, dataw, byt_sel);
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

int get_riscof_en(int z) {
    return riscof;
}

int get_end_riscof(int z) {
    return rvtest_code_end;
}


extern int ghdl_main(int argc, char const* argv[]);


int main(int argc, char const* argv[]) {
    
    char   signature_name[200] ="";
    char   opt[20] = "";
    char   input_file[50] ;
    char   output[50] ;
    char   test[512] = "> a.out.txt";
    int nargs = 1;
    int rvtest_entry_point = 0;


    strcpy(input_file,argv[1]) ;
    strcpy(output,argv[1]) ;
    // Receiving arguments
    if (argc >= 3 && strcmp(argv[2],"-O") == 0) {
        nargs = 2;
        strcpy(opt,"-02") ;
    } else if (argc >= 4 && strcmp(argv[2],"--riscof") == 0) {
        nargs = 3;
        strcpy(signature_name,argv[3]);
        riscof         = 1;
    }

    // Getting riscof signature file name :

    if(strcmp(signature_name,"") !=0){
        riscof_signature = fopen(signature_name,"w") ;
        if( riscof_signature == NULL)
        {
            fprintf(stderr, "error while opening %s\n", signature_name);
            exit(1) ;
        }
        else{
            fprintf(stderr, "Opening %s was successfull\n", signature_name);
        }
    }

    char temp_text[512];
    char point = '.' ;
    char *type_of_file = strrchr(input_file,point) ; 

    // Generation of executable file

    if(strcmp(type_of_file,".c") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld %s",
                input_file);
        system((char*)temp);
        strcpy(output,"a.out") ;
    }  
    if(strcmp(type_of_file,".s") == 0){
        char temp[512] ;
        sprintf(temp,"riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld %s",
                input_file);
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
            //printf("%8x : %8x\n",(pObj->Section_Hdr[i]->sh_addr)+j, mem_lw(pObj->Section_Hdr[i]->sh_addr+j)) ;
            write_mem((pObj->Section_Hdr[i]->sh_addr)+j,mem_lw(pObj->Section_Hdr[i]->sh_addr+j), 15);
        }
    }

    if(Elf32_SymAdr(pObj,&begin_signature,"begin_signature")==0)
        fprintf(stderr, "Found begin_signature at : 0x%8x\n", begin_signature);
   
    if(Elf32_SymAdr(pObj,&rvtest_code_end,"rvtest_code_end")==0)
        fprintf(stderr, "Found rvtest_code_end at : 0x%8x\n", rvtest_code_end);
   
    if(Elf32_SymAdr(pObj,&rvtest_entry_point,"rvtest_entry_point")==0)
            fprintf(stderr, "Found rvtest_entry_point at : 0x%8x\n", rvtest_entry_point);
   
    if(Elf32_SymAdr(pObj,&end_signature,"end_signature")==0)
                fprintf(stderr, "Found end_signature at : 0x%8x\n", end_signature);
   
    if(Elf32_SymAdr(pObj,&exception_adr,"_exception_occur")==0)
                fprintf(stderr, "Found _exception_occur at : 0x%8x\n", end_signature);
   

    if(rvtest_entry_point)
        start_pc = rvtest_entry_point;
    else
        start_pc = 0x80000000;
        //start_pc = (structure->start_adr);
        //  
        
        
    printf("Start Adress : %x\n",start_pc) ;

    ghdl_main(argc - nargs, &argv[nargs]);
    return 0 ;
}

