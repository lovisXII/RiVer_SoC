#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NOC='\033[0m'
make core_tb
for file in $(ls ../../SOFT/TESTS/I/); do 
    printf "Test ${file} non opt..." 
    timeout 30s make run TEST=../../SOFT/TESTS/I/$file >/dev/null 2>&1;
    RES=$?;
    if (($RES == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    elif (($RES == 2))
    then
        printf "${YELLOW} exception occured\n${NOC}"
    else 
        printf "${RED} failed\n${NOC}"    
    fi
    printf "Test ${file} opt..." 
    timeout 30s make run TEST=../../SOFT/TESTS/I/$file -O >/dev/null 2>&1;
    RES=$?;
    if (($RES == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    elif (($RES == 2))
    then
        printf "${YELLOW} exception occured\n${NOC}"
    else
        printf "${RED} failed\n${NOC}" 
    fi

done
for file in $(ls ../../SOFT/TESTS/M/); do 
    printf "Test ${file} non opt..." 
    timeout 30s make run TEST=../../SOFT/TESTS/M/$file >/dev/null 2>&1;
    RES=$?;
    if (($RES == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    elif (($RES == 2))
    then
        printf "${YELLOW} exception occured\n${NOC}"
    else 
        printf "${RED} failed\n${NOC}"    
    fi
    printf "Test ${file} opt..." 
    timeout 30s make run TEST=../../SOFT/TESTS/M/$file -O >/dev/null 2>&1;
    RES=$?;
    if (($RES == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    elif (($RES == 2))
    then
        printf "${YELLOW} exception occured\n${NOC}"
    else
        printf "${RED} failed\n${NOC}" 
    fi

done
