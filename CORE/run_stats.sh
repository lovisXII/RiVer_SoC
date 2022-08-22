#!/bin/bash
make -j
rm *.txt
for file in $(ls ../TESTS/I/); do 
    printf "Test ${file} ...\n" 
    timeout 20s ./core_tb ../TESTS/I/$file --stats>/dev/null>/dev/null 2>&1 ;
done
mkdir -p ../TMP/
cp *.txt ../TMP