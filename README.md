# RISC-V-project

This README file isn't complete, everything here is subject to change. 

# Introduction :

This project started as a project of our 1st year Master and aimed to modernise the architecture studied at Sorbonne University.\
We were offered to continue this project during a 3 month Internship from June 2022 to August 2022.\
This project is the result of about 8 months of works.\
It was realised by :

**Timothée Le Berre**,\
**Louis Geoffroy Pitailler**,\
**Kevin Lastra**,\
**Samy Attal**


For more than a decade, class from Master SESI used MIPS32 architecture. In this project we aim to create a material description of a RISCV architecture based on the 5 stage pipeline MIPS32 used in class.\
The implementation uses the standard instruction set from [RISCV fondation](https://riscv.org/technical/specifications/). We choosed to implement a **RV32IM** with **Zicsr** extension and a **user** and **machine** mode. On this git you will find :
- A RISCV 5 stages scalar processor in SystemC
- A RISCV 5 stages scalar processor in VHDL
- A RISCV 2 ways super-scalar processor in 
SystemC
- A Soc prototype in SystemcC using 2 scalar CORE
- Drivers and hardware 

All of our core are fully operationnal and have been tested using riscof framework and custom tests that you can find in the **SOFT/TESTS/**

Our Kernel architecture supports :
* direct and vectorize mode for mtvec
* it stores the adresse of an instruction responsable of an exception inside mepc
* it stores the value responsable of an exception inside mtval (for instance bad load adress value, missaligned instruction value...etc )

# I. How to compile ?
## A. What's needed 

To compile the project you will need **Systemc version 2.3.3**, g++ and a riscv compiler.
You will find shell script in **Shell_script/** to help you to setup your envirroment. If you trust us enough you can skip this part and go read helper script. Otherwise follow these istruction

### SystemC
```bash
sudo apt install build-essential make wget git gcc g++ automake
wget http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.gz
tar -xzf systemc-2.3.3.gz
mkdir -p systemc-2.3.3/objdir
cd systemc-2.3.3/objdir
../configure --prefix=/usr/local/systemc-2.3.3/
make -j
sudo make install
```
If you use another path for SystemC, you will need to set the SYSTEMC env variable before compilation.

### riscv32 cross-compiler
*pre-compiled binaries on Ubuntu*
```bash
wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
sudo mkdir -p /opt/riscv
sudo tar -xzf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C /opt/riscv/
rm -f riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
```
Then add it to your PATH by adding the following line to `~/.bashrc` : 
```bash
export PATH=$PATH:/opt/riscv/bin 
```
*re-compiled (safer and works for most systems)*

Follow the instructions here https://github.com/riscv-collab/riscv-gnu-toolchain to build a 32-bit riscv toolchain for freestanding code. 



### helper script

If you trust us enough to run ou script in sudo, then we provide helper scripts (we provide no guarantee the installation will be clean, but it should work).

Several case can occur :
- if you only want to compile one of our SystemC core and you want to play with it, you will need to run :
```
./install_systemc.sh
./install_riscv.sh
```
- if you want to use riscof framework, you will need to run :
```
./install_systemc.sh
./install_riscv.sh
./install_riscof.sh
```
**Please note that if you are running on ubuntu 22.04 you can have issue while installing riscof because of python version. indeed you need python3.6 overwhise it will not works.**
To avoid this problem, we wrote **install_python_ub_22_04.sh**, so please run it then run riscof.
- if you want to run the vhdl core using ghdl, you will need to run 
```
./install_riscv.sh
./install_ghdl.sh
```

### building the project

Once everything is installed you will have to go into ``SIM/``.
You will find :
- ``CORE_VHDL/`` : it contains the vhdl description of a RV32IM 5 stages scalar pipelined processor with branch prediction.
- ``ELFIO/`` : it contais a library we used in our SystemC implementation to parse an elf file.
- ``SystemC/`` : it contains 2 cores and the Soc descriptio :
    - ``CORE/`` : same implementation than the vhdl one but i systemC.
    - ``CORE_SS2/`` : a 5 stages, 2 way super-scalar RV32I with user and machine mode.
    - ``SOC/`` : the soc description

 Once you are in the right directory, you juste have to do a ```make```. It will generate the executable core_tb.\
This file takes as argument an assembly file or a c one, once you pass it the file it will executes it using our descritption of a RISCV core.\
You will find some test programm in ``/SOFT/TESTS/``.

For example you can run ``core_tb ../../../SOFT/TESTS/I/pgcd.c``.

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

If you are completly new to processor architecture here's a quick tips to start reading our file, start in the following order :
* ``CORE/EXEC`` : this stage is responsable of all logic operation done by the processor
* ``CORE/MEM`` : this stage does the memory access like store and word operation
* ``CORE/WBK`` : it stores result of an operation into REG
* ``CORE/REG`` : this is the bank register which contains all the CPU's register which store data used for all operations
* ``CORE/IFETCH`` : this stage is an interface with the cache, it loads instruction thanks to the PC (program counter) send by DEC
* ``CORE/DEC`` : this is the most complex file, it contains almost all the processor logic, it decodes instructions, handles jumps, ...
