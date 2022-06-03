#! /bin/bash

# Please update the path where you want to install the gnu toolchain for riscv
export TEMPORARY_PATH=/home/louis/riscof_setup 

echo "Please run in sudo"
echo "Installing python"

sudo apt-get install python3.6
pip3 install --upgrade pip
sudo add-apt-repository ppa:deadsnakes/ppa
sudo apt-get update
sudo apt-get install python3.6 -y
pip3 install --upgrade pip

echo "Installing riscof"

pip3 install git+https://github.com/riscv/riscof.git

echo "Installing GNU Toolchain"

sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev \
libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool \
patchutils bc zlib1g-dev libexpat-dev
git clone --recursive https://github.com/riscv/riscv-gnu-toolchain
git clone --recursive https://github.com/riscv/riscv-opcodes.git
cd riscv-gnu-toolchain
./configure --prefix=$TEMPORARY_PATH --with-arch=rv32gc --with-abi=ilp32d # for 32-bit toolchain
make -j4 # sudo is required depending on the path chosen in the previous setup

echo "Installing spike"

sudo apt-get install device-tree-compiler
git clone https://github.com/riscv-software-src/riscv-isa-sim.git
cd riscv-isa-sim
mkdir build
cd build
../configure --prefix=$TEMPORARY_PATH
make -j4
sudo make install #sudo is required depending on the path chosen in the previous setup