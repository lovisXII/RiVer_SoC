# RISC-V-project
# Introduction :
This project as for purpose to modernise the architecture studied at Sorbonne University.\
It was realised by Timothée Le Berre, Louis Geoffroy Pitailler & Kevin Lastra during their first year of Master SESI.\
Since one decate or more, class from Master SESI used MIPS32 architecture. In this project our purpose is to create a material description of a RISCV architecture based on a 5 stage pipeline MIPS32.\
To do this we used the riscv spec and we used the mips 5 stage pipele as base for our design.

![plot](RiscV_graph.PNG)

# I. How to compile ?
## A. What's needed 

To compile the project you ll need **Systemc version 2.3.3**, g++ and a riscv compiler.\
To help you install all of this you will find two scripts in ```/Shell_script/``` :
* install_riscv.sh : it will install the riscv compiler needed, **need to be in sudo**,
* install_systemc.sh : it will install Systemc.2.3.3, **need to be in sudo**

Once everything is installed you will have to do a ```make``` in the directory ```/CORE/```. It will generate the executable core_tb.\
This file takes has argument an assembly file or a c one, once you pass him the file it will execute it using our descritption of a RISCV core.\
You will find some test programm in ``/CORE/tests``.

For example you can run ``core_tb tests/pgcd.c``.

We also write a shell script ``run_all_tests.sh`` that take all the file inside tests/ and execute it and print a message saying if the exectution was a success or not.

## B. Directory map :

This project is design as follow :
* Compte_rendu : Contains a simplify spec of basic RISCV32 instructions set,
* CORE : Contains every directory needed for the architecture,
* Doc_useful : Contains some pdf that we find usefull to help us in our project,
* ELFIO : c++ library that provides a simple interface fir reading and generating files in ELF binary format,
* MIPS : old description of a MIPS32 architecture made byt the lip6 of Sorbonne Unversity, it used an old version of systemc, we modify it to be able to compile on systemc2.3.3,
* Risc-sim : contain the begining of a parsor to transfrom a assembly file into binary code
* tests : Contains some programm that are kind of test, we will move them in a permanent directory at the end of the project

```
├── CORE : Contains all the directory needed and the test bench for the whole core
│   ├── CACHES : Instruction cache and Data cache
│   ├── DEC : Decod stage
│   ├── EXE : execute stage
│   ├── IFETCH : ifetch stage 
│   ├── MEM : memory stage
│   ├── REG : bank register
│   ├── UTIL : useful stuff such as fifo
│   ├── WBK : write back stage
│   └── tests : all tests for our design (.c and .s)
|
├── Documentation : usefull documents and reports
│   ├── Reports : reports of the whole project
│   ├── Todo_kernel : a little file saying what we plan to do for the kernel
│   └──Usefull_documentation : all the doc usefull
| 
├── ELFIO : c++ elfio library
├── MIPS : an old descritpion of MIPS32 architecture in systemc
├── Shell_script : script shell for the setup of the environment
└── tests : all kind of test files
```
# II. Internal Architecture
## A. Convetion :

1. **Entity** :

Signals of pure entity such as arithemitcal logic unit, shifter, fifo ...etc. It is named according to the following rules :  
 - **Upper case** : designate the input/ output of an entity
- **Lower case** : internal signals of an entity  

The name of the signal will be just a quick description of what the signal do.  We don't use the same conventions as the Stage because "pure entity" can be instantiated at different stages so it would not make sens to name it according to a stage.  
__example__ :  
- DIN : alu's data input  
- RES : alu's result  
- CMD :  alu's command  

2. **Stage :**

Our signals are name according to the following convention :  

__**Description_XY**__  

- **Description** : a quick description of what the signal means,  
- **X** : Signal type, can be a register or just a pure signal :
    * S for signal,
    * R for register
- **Y** : The initial letter of the stage it is coming from :
    * I : IFETCH,
    * D : DECOD,
    * E : EXEC,
    * M : MEM,
    * W : WBK

- **Upper case** : designate the input/ output of an entity
- **Lower case** : internal signals of an entity  

__example__ :  
    dec_to_exe_cin_stg1 : designate the signal carry in coming from decode and going to exe of the first pipeline.

## B. Description of all the signals :

### 1. IFETCH 

```txt

    // Interface with ICACHE :

    ADR_SI              : address sent to the ICACHE
    ADR_VALID_SI        : say if the adress is valid or no

    IC_INST_SI          : Instruction coming from the Cache
    IC_STALL_SI         : Signal saying if we need to stall the stage or no
    

    //dec2if interface :
    
    DEC2IF_EMPTY_SI     : Indicate if the fifo is empty or no 
    DEC2IF_POP_SI       : Signal going to decod and allowing it to pop the fifo to get the instruction

    //if2dec interface
    
    IF2DEC_FLUSH_SD     :
    IF2DEC_POP_SD       :
    IF2DEC_PUSH_SI      :
    IF2DEC_FULL_SI      :
    IF2DEC_EMPTY_SI     :
    
	PC_RD 		        :
	INSTR_RI 		    :
    
    //Global Interface :

	CLK 		        :
	RESET 		        :
```

### 2. DECOD 

```C
   //Interface with REG :

	RDATA1_SR 		    :
	RDATA2_SR 		    :

	RADR1_SD 		    :
	RADR2_SD 		    :

	WRITE_PC_SD 		:
	WRITE_PC_ENABLE_SD 	:

	READ_PC_SR 		    :

    //Interface with EXE :

	OP1_RD 		        :
	OP2_RD 		        :
	EXE_CMD_RD 		    :
	NEG_OP2_RD 		    :
	WB_RD 		        :
	EXE_DEST_SD 		:    //the destination register
	SELECT_SHIFT_RD 	:
	SLT_RD 		        :
	SLTU_RD 		    :

	MEM_DATA_RD 		:
	MEM_LOAD_RD 		:
	MEM_STORE_RD 		:
	MEM_SIGN_EXTEND_RD 	:
	MEM_SIZE_RD 		:

    
    // Interface with DEC2IF : 

	DEC2IF_POP_SI 		:
	DEC2IF_EMPTY_SD 	:
	PC_RD 		        :

    //Interface with IF2DEC :

	INSTR_RI 		    :
	IF2DEC_EMPTY_SI 	:
	IF2DEC_POP_SD 		:
	IF2DEC_FLUSH_SD 	:

    //Interface with DEC2EXE

	DEC2EXE_POP_SE 		:
	DEC2EXE_EMPTY_SD 	:
	DEC2EXE_OUT_SD 		:

    //Bypasses
	BP_DEST_RE 		    :
	BP_EXE_RES_RE 		:
	BP_MEM_LOAD_RE 		:
	BP_EXE2MEM_EMPTY_SE :
	BP_DEST_RM 		    :
	BP_MEM_RES_RM 		:


	BP_R1_VALID_RD 		:
	BP_R2_VALID_RD 		:
	BP_RADR1_RD 		:
	BP_RADR2_RD 		:

    //General Interface :
	CLK 		        :
	RESET_N 		    :


    //Instance used :
    
	fifodec2if 		    :
	fifodec2exe 		:

    // Signals :

	rdata1_sd 		    :
	rdata2_sd 		    :
	r1_valid_sd 		:
	r2_valid_sd 		:
	stall 		        :
	block_in_dec 	    :

    //fifo dec2if :
    
	dec2if_in_sd 		:
	dec2if_push_sd 		:
	dec2if_empty_sd 	:
	dec2if_full_sd 		:
	dec2if_out_sd 		:

    //fifo dec2exe :

	dec2exe_in_sd 		:
	dec2exe_push_sd 	:
	dec2exe_full_sd 	:

    // Instruction format type :

	r_type_inst_sd 		:
	i_type_inst_sd 		:
	s_type_inst_sd 		:
	b_type_inst_sd 		:
	u_type_inst_sd 		:
	j_type_inst_sd 		:
	jalr_type_inst_sd 	:

    //R-type Instructions :


	add_i_sd 		    :
	slt_i_sd 		    :
	sltu_i_sd 		    :
	and_i_sd 		    :
	or_i_sd 		    :
	xor_i_sd 		    :
	sll_i_sd 		    :
	srl_i_sd 		    :
	sub_i_sd 		    :
	sra_i_sd 		    :

    //I-type Instructions :

	addi_i_sd 		    :
	slti_i_sd 		    :
	sltiu_i_sd 		    :
	andi_i_sd 		    :
	ori_i_sd 		    :
	xori_i_sd 		    :

	jalr_i_sd 		    :

    // I-type shift instructions :

	slli_i_sd 		    :
	srli_i_sd 		    :
	srai_i_sd 		    :

   // I-type load instructions :

	lw_i_sd 		    :
	lh_i_sd 		    :
	lhu_i_sd 		    :
	lb_i_sd 		    :
	lbu_i_sd 		    :

    //B-type Instruction :

	beq_i_sd 		    :
	bne_i_sd 		    :
	blt_i_sd 		    :
	bge_i_sd 		    :
	bltu_i_sd 		    :
	bgeu_i_sd 		    :

    //U-type Instruction :

	lui_i_sd 		    :
	auipc_i_sd 		    :

    //J-type Instruction :  

	jal_i_sd 		    :

    //S-type Instructions :

	sw_i_sd 		    :
	sh_i_sd 		    :
	sb_i_sd 		    :

    //Offset for branch :

	offset_branch_sd 	:

    //PC gestion :

	inc_pc_sd 		    :
	add_offset_to_pc_sd :


    //Internal signals :

	adr_dest_sd 		:
	exe_op1_sd 		    :
	exe_op2_sd 		    :
	mem_data_sd 		:
    
	mem_size_sd 		:
	mem_load_sd 		:
	mem_store_sd 		:

	exe_cmd_sd 		    :
	select_shift_sd 	:
	exe_neg_op2_sd 		:
	exe_wb_sd 		    :
	mem_sign_extend_sd 	:
```

### 3. EXE 

```c
     // Input/Output of EXE : 

	OP1_RD 		        :
	OP2_RD 		        :
	OP1_VALID_RD 		:
	OP2_VALID_RD 		:
	RADR1_RD 		    :
	RADR2_RD 		    :

	MEM_DATA_RD 		:
	DEST_RD 		    :
	CMD_RD 		        :
	MEM_SIZE_RD 		:
    NEG_OP2_RD, WB_RD   :
    MEM_SIGN_EXTEND_RD  :
    SELECT_SHIFT_RD 	:
    MEM_LOAD_RD         :
    MEM_STORE_RD 		:
    EXE2MEM_POP_SM      :
    DEC2EXE_EMPTY_SD 	:
    SLT_RD,	SLTU_RD 	:
	CLK 		        :
	RESET 		        :
    
    //Fifo exe2mem interface :

	EXE_RES_RE 		    :
	MEM_DATA_RE 		:
	DEST_RE 		    :
	MEM_SIZE_RE 		:

    WB_RE               :
    MEM_SIGN_EXTEND_RE 	:
    MEM_LOAD_RE         :
    MEM_STORE_RE 		:
    EXE2MEM_EMPTY_SE    :
    DEC2EXE_POP_SE 		:
    

    //Internals signals :
    
	exe_res_se 		    :

	exe2mem_din_se 		: // concatenation of exe_res, mem_data...etc
	exe2mem_dout_se 	:

	op1_se 		        :
	op2_se 		        :
	alu_in_op2_se 		:
	alu_out_se 		    :
	shifter_out_se 		:
	bp_mem_data_sd 		:
	shift_val_se 		:  
    
    exe2mem_push_se     :
    exe2mem_full_se 	:  
	blocked 		    :    

    //bypasses

	MEM_DEST_RM 		:
	MEM_RES_RM 		    :
```

### 3. MEM

```c
    // Mcache Interface :

	MCACHE_ADR_SM 		:
	MCACHE_DATA_SM 		:
    MCACHE_ADR_VALID_SM :
    MCACHE_STORE_SM     :
	MCACHE_LOAD_SM 		:

	MCACHE_RESULT_SM 	:
	MCACHE_STALL_SM 	:
    
    //Exe Interface :

	EXE_RES_RE 		    :
	MEM_DATA_RE 		:
	DEST_RE 		    :
	MEM_SIZE_RE 		:

    WB_RE               :
	SIGN_EXTEND_RE 		:
    LOAD_RE             :
	STORE_RE 		    :

    //exe2mem interface :
    
	EXE2MEM_EMPTY_SE 	:
	EXE2MEM_POP_SM 		:

    //mem2wbk interface

	MEM2WBK_POP_SW 		:
	mem2wbk_push_sm 	:
	mem2wbk_full_sm 	:
	MEM2WBK_EMPTY_SM 	:
    

    //WBK interface 
	MEM_RES_RM 		    :
	DEST_RM 		    :
	WB_RM 		        :
    
    //Internal signals
	mem2wbk_din_sm 		:
	mem2wbk_dout_sm 	:
	data_sm 		    :
	wb_sm 		        :
    //Global Interface :

	CLK 		        :
	RESET 		        :
```

### 4. WBK

```c
     // Reg Interface :

	WADR_SW 		    :
	WDATA_SW 		    :
	WENABLE_SW 		    :
    
    //Mem Interface :

	MEM_RES_RM 		    :
	DEST_RM 		    :
	WB_RM 		        :
    //Mem2wbk interface

	MEM2WBK_EMPTY_SM 	:
	MEM2WBK_POP_SW 		:

    //Internal signals

    //Global Interface :

	CLK 		        :
	RESET 		        :
```

### 5. REG

```c
    // Reading Port :

	RADR1_SD 		    :
	RADR2_SD 		    :

	RDATA1_SR 		    :
	RDATA2_SR 		    :

    // Writing Port :

	WADR_SW 		    :
	WENABLE_SW 		    :
	WDATA_SW 		    :

	WRITE_PC_SD 		:
	WRITE_PC_ENABLE_SD 	:


    //PC Gestion :

	READ_PC_SR 		    :

    //Global Interface :

	CLK 		        :
	RESET_N 		    :
	PC_INIT 		    :

    //Registres :
    REG_RR[33]          :
```
