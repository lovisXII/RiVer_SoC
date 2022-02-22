# RISC-V-project
# Introduction :
This project as for purpose to modernise the architecture studied at Sorbonne University. Since one decate or more, class from Master SESI used MIPS32 architecture. In this project our purpose is to create a material description of a RISCV architecture based on a 5 stage pipeline MIPS32.\
To do this we used the riscv spec and we used the mips 5 stage pipele as base for our design.\
This project is design as follow :
* bin : Contains some programm that are kind of test, we will move them in a permanent directory at the end of the project,
* Compte_rendu : Contains a simplify spec of basic RISCV32 instructions set,
* CORE : Contains every directory needed for the architecture,
* Doc_useful : Contains some pdf that we find usefull to help us in our project,
* ELFIO : c++ library that provides a simple interface fir reading and generating files in ELF binary format,
* MIPS : old description of a MIPS32 architecture made byt the lip6 of Sorbonne Unversity, it used an old version of systemc, we modify it to be able to compile on systemc2.3.3,
* Risc-sim : contain the begining of a parsor to transfrom a assembly file into binary code


## I. Convention we took to name signals
### A. Entity :
Signals of pure entity such as arithemitcal logic unit, shifter, fifo ...etc. are named according to the following rule :  
 - **Upper case** : designate the input/ output of an entity
- **Lower case** : internal signals of an entity  

The name of the signal will be just a quick description of what the signal do.  We didn't use the same convention as the Stage because "pure entity" can be instantiated at different stages so it would not make sens to name it according to a stage.  
__example__ :  
- DIN : alu's data input  
- RES : alu's result  
- CMD :  alu's command  
### B. Stage : 
Our signals are name according to the following convention :  

__**From_to_description_stage**__  

- **From** : designate the stage where the signal is coming from,  
- **to** : designate the stage into the one the signal goes,  
- **description** : a quick description of what the signal means,  
- **stage** : designate to which pipeline the signal belongs  

For the name of signals two case can occure :  

- **Upper case** : designate the input/ output of an entity
- **Lower case** : internal signals of an entity  

__example__ :  
    dec_to_exe_cin_stg1 : designate the signal carry in coming from decode and going to exe of the first pipeline.


