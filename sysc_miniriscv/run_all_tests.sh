#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NOC='\033[0m'
make -j
riscv32-unknown-elf-gcc -nostdlib -c SW/reset.s -o reset.o  >/dev/null 2>&1;
tabs 45 55;
for file in $(ls tests/); do 
    printf "Test ${file} non opt..." 

    riscv32-unknown-elf-gcc -nostdlib -c tests/$file -o app.o  >/dev/null 2>&1;
    riscv32-unknown-elf-gcc -nostdlib reset.o app.o -T SW/link.ld -o app  >/dev/null 2>&1;
    timeout 20s ./run.exe app >/dev/null 2>&1;
    RES=$?;
    if (( $RES == 0 ))
    then
        printf "${GREEN} passed\n${NOC}"
    elif (( $RES == 2 ))
    then
        printf "${YELLOW} exception\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi

done
tabs -0


