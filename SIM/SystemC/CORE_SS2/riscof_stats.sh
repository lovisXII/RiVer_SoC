#!/bin/bash
make -j
rm *.txt
for file in $(ls ../../../riscof/riscof_work/rv32i_m/I/src/*/dut/my.elf); do  
    printf "Test ${file} ...\n"
    timeout 60s ./core_tb $file --riscof a.txt>/dev/null>/dev/null 2>&1 ;
done
# for file in $(ls ../riscof/riscof_work/rv32i_m/privilege/src/*/dut/my.elf); do  
#     printf "Test ${file} ...\n"
#     timeout 60s ./core_tb $file --riscof a.txt>/dev/null>/dev/null 2>&1 ;
# done
mkdir -p ../../../TMP/
cp *.txt ../../../TMP