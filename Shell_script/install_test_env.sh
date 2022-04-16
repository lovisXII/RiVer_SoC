#! /bin/bash

pip3 install git+https://github.com/riscv/riscof.git
cd /tmp
apt-get install device-tree-compiler
git clone https://github.com/riscv-software-src/riscv-isa-sim.git
cd riscv-isa-sim
mkdir build
cd build
../configure --prefix=/opt/spike
make 
make install

if grep -q microsoft /proc/version; then
  echo "Ubuntu on Windows"
  echo "export PATH=$PATH:/opt/spike/bin" >> ~/.bashrc
  source ~/.bashrc
else
  echo "Native Linux"
  echo "export PATH=$PATH:/opt/spike/bin" >> ~/.bashrc
  source ~/.bashrc
fi