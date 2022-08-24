#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NOC='\033[0m'
make -j
for file in $(ls ../../../SOFT/TESTS/I/); do 
    printf "Test ${file} non opt..." 
    timeout 5s ./core_tb ../../../SOFT/TESTS/I/$file >/dev/null 2>&1
    if (($? == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi
    printf "Test ${file} opt..." 
    timeout 5s ./core_tb ../../../SOFT/TESTS/I/$file -O >/dev/null 2>&1
    if (($? == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi

done