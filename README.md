# RISC-V-project

This README file isn't complete, everything here is subject to change. 

# Introduction :

This project started as a project of our 1st year Master and aimed to modernise the architecture studied at Sorbonne University.\
We were offered to continue this project during a 3 month Internship from July 2022 to August 2022.\
This project is the result of about 8 months of works.\
It was realised by :

**Timothée Le Berre**,\
**Louis Geoffroy Pitailler**,\
**Kevin Lastra**,\
**Samy Attal**


For more than a decade, class from Master SESI used MIPS32 architecture. In this project we aim to create a material description of a RISCV architecture based on the 5 stage pipeline MIPS32 used in class.\
The implementation uses the standard instruction set from [RISCV fondation](https://riscv.org/technical/specifications/). We choosed to implement a **RV32IM** with **Zicsr** extension and a **user** and **machine** mode.

Our Kernel architecture supports :
* direct and vectorize mode for mtvec
* it stores the adresse of an instruction responsable of an exception inside mepc
* it stores the value responsable of an exception inside mtval (for instance bad load adress value, missaligned instruction value...etc )

# I. How to compile ?
## A. What's needed 

To compile the project you will need **Systemc version 2.3.3**, g++ and a riscv compiler.\

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

If you trust us enough to run ou script in sudo, then we provide helper scripts (we provide no guarantee the installation will be clean, but it should work) : 
* install_riscv.sh : it will install the riscv compiler needed, **need to be in sudo**,
* install_systemc.sh : it will install Systemc.2.3.3, **need to be in sudo**

### building the project

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

If you are completly new to processor architecture here's a quick tips to start reading our file, start in the following order :
* ``CORE/EXEC`` : this stage is responsable of all logic operation done by the processor
* ``CORE/MEM`` : this stage does the memory access like store and word operation
* ``CORE/WBK`` : it stores result of an operation into REG
* ``CORE/REG`` : this is the bank register which contains all the CPU's register which store data used for all operations
* ``CORE/IFETCH`` : this stage is an interface with the cache, it loads instruction thanks to the PC (program counter) send by DEC
* ``CORE/DEC`` : this is the most complex file, it contains almost all the processor logic, it decodes instructions, handles jumps, ...
