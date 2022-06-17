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
The implementation use the standard instruction set from [RISCV fondation](https://riscv.org/technical/specifications/). We choosed to implement a **RV32IM** with **Zicsr** extension and a **user** and **machine** mode.

Our Kernel architecture supports :
* direct and vectorize mode for mtvec
* it stores the adresse of an instruction responsable of an exception inside mepc
* it stores the value responsable of an exception inside mtval (for instance bad load adress value, missaligned instruction value...etc )

# I. How to compile ?
## A. What's needed 

To compile the project you will need **Systemc version 2.3.3**, g++ and a riscv compiler.\
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

If you want details about the Internal architecture of our implemntation please go inside ``Documentation/Reports/Rapport_PSESI.pdf``.