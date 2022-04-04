#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NOC='\033[0m'

make -j 10
for file in $(ls tests); do 
    printf "Test ${file}..." 
    ./core_tb tests/$file >/dev/null 2>&1
    if (($? == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi

done
