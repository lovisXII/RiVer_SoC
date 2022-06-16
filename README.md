# RISC-V-project

This README file isn't complete, everything here is subject to modification. 

# Introduction :

This project was introduce as a project of our 1st year Master and had for purpose to modernise the architecture studied at Sorbonne University.\
We were offer to continue this project during a 3 month Internship from July 2022 to August 2022.\
This project is the result of about 8 months of works.\
It was realised by :

**Timothée Le Berre**,\
**Louis Geoffroy Pitailler**,\
**Kevin Lastra**,\
**Samy Attal**


Since one decate or more, class from Master SESI used MIPS32 architecture. In this project our purpose is to create a material description of a RISCV architecture based on a 5 stage pipeline MIPS32.\
The implementation use the standard instruction set from [RISCV fondation](https://riscv.org/technical/specifications/). We choosed to implement a **RV32IM** with **Zicsr** extension and a **user** and **machine** mode.\

Here's a schema of the 5 stage pipeline of our architecture without the Kernel mode :

![plot](Documentation/Reports/Pictures_for_reports/RiscV_graph.PNG)


To find out more about it, please go into ``Documentation/Reports/`` and read **PSESI_final_report**.

# I. How to compile ?
## A. What's needed 

To compile the project you ll need **Systemc version 2.3.3**, g++ and a riscv compiler.\
To help you install all of this you will find two scripts in ```/Shell_script/``` :
* install_riscv.sh : it will install the riscv compiler needed, **need to be in sudo**,
* install_systemc.sh : it will install Systemc.2.3.3, **need to be in sudo**

Once everything is installed you will have to do a ```make``` in the directory ```/CORE/```. It will generate the executable core_tb.\
This file takes as argument an assembly file or a c one, once you pass it the file it will executes it using our descritption of a RISCV core.\
You will find some test programm in ``/CORE/tests``.

For example you can run ``core_tb tests/pgcd.c``.

We also write a shell script ``run_all_tests.sh`` that take all the file inside tests/ and execute it and print a message saying if the execution was a success or not.

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
## A. Convention :

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

## B. Kernel Architecture

![plot](Documentation/Reports/Pictures_for_reports/schema_pipeline_exception.jpg)

Our implementation support a machine mode (11) and user mode (00). To implement this, we needed to add the Zicsr extension and we needed to choose which csr to implement.

We choosed to add a specific adress which delimitate the machine section of the memory segments. You will find a **define inside EXE/exec.h** which indicate the start of the reserved machine adress.\
``By default it is sets at 0xF0000000``

We implemented the following Csr :
* 0: mvendorid : not implemented so 0
* 1: marchid : not implemented so 0
* 2: mimpid : not implemented so 0
* 3: mstatus : gives the current status of the CPU
* 4: misa : gives the current extension implemented in the core
* 5: mie : gives the interruption that are mask 
* 6: mtvec : gives the adress of the trap handler, our implementation supports a **direct** and **vectorise** configuration
* 7: mstatush : msb of mstatus
* 8: mepc : stores the adress of an instruction which occur an exception
* 9: mcause : stores the code value of an exception
* 10: mtval : stores the adress that generate an access fault or an adress missaligned excption 
* 11: mip : bit vector that indicate pending interruption 
* 12: mscratch : hold a pointer to a machine-mode hart-local context space and swapped
with a user register upon entry to an M-mode trap handler

These CSR can be found inside the csr.h file. 

An exception can occur in any stage of the pipeline and it must be deadly for the current execute program. To be sure of this we detects exception in our stage **MEM** just before the memory accesses, if an exception occur all memories access are disable.

The current exception can occur :
* Instruction **adress missaligned** after a branch instruction : detected in DEC
* Instruction **adress fault**, meaning we're trying to access a machine reserved section in user mode : detected in DEC
* **Illegal** Instruction : detected in DEC
* Environment call since **User-mode** : detected in DEC
* Environment call since **Machine-mode** : detected in DEC
* **Ebreak** instruction : detected in DEC
* mret, we deal with this instruction as if it was an exception, this instruction can only be used since a machine mode pipeline. So if a user program try to use this instruction it will raise an exception which will be detected in **DEC**
* Load or store Missaligned : **EXE**
* Load or Store access fault : **EXE**
