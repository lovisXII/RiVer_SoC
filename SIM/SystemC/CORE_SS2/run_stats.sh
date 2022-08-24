#!/bin/bash
make -j
rm *.txt
for file in $(ls ../../../SOFT/TESTS/I/); do 
    printf "Test ${file} ...\n" 
    timeout 20s ./core_tb ../../../SOFT/TESTS/I/$file --stats>/dev/null 2>&1;
done
mkdir -p ../../../TMP/
cp *.txt ../../../TMP