#!/bin/bash
make -j
for file in $(ls ../TESTS/tests/); do 
    printf "Test ${file} ...\n" 
    timeout 20s ./core_tb ../TESTS/tests/$file --stats>/dev/null 2>&1;
done
tabs -0