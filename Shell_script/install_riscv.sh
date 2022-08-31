#!/bin/bash

echo "This script will installed precompiled binary file for riscv cross-compiler"
echo "Please run in sudo"

if ( cat /etc/*-release | grep Ubuntu > /dev/null 2>&1) then
  rm riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
  rm riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz.*
  wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
  mkdir -p /opt/riscv
  tar -xzf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C /opt/riscv/
  echo "export PATH=$PATH:/opt/riscv/bin" >> ~/.bashrc
  source ~/.bashrc
  rm -f riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
fi