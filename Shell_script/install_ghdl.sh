#! /bin/bash

echo "Installing dependencies"
apt-get install libedit clang lldb lld pkg-config zlib1g-dev

echo "Downloading ghdl sources"
wget https://github.com/ghdl/ghdl/archive/master.tar.gz
tar -xvf ghdl-master.tar.gz
cd ghdl-master && mkdir -p build 
cd build && ../configure --with-llvm-config --prefix=/opt/ghdl
make 
make install 

echo "export ghdl=/opt/ghdl/bin/ghdl" >> ~/.bashrc
source ~/.bashrc

# https://ghdl.github.io/ghdl/development/building/LLVM.html