#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NOC='\033[0m'
make -j
tabs 45 55;
for file in $(ls ../TESTS/tests/); do 
    printf "Test ${file} non opt..." 

    timeout 20s ./core_tb ../TESTS/tests/$file >/dev/null 2>&1;
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
    printf "Test ${file} opt..." 
    timeout 20s ./core_tb ../TESTS/tests/$file -O >/dev/null 2>&1
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