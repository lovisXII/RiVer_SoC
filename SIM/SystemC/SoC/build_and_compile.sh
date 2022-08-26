make clean ; 
make -j && 
riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T SW/app.ld ../../../SOFT/TESTS/Multi_CORE/back_and_return.c ../../../SOFT/TESTS/Multi_CORE/lib.c && 
./SoC_tb a.out 
