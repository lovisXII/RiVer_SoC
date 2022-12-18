#! /bin/bash

echo "Please run in sudo"
echo "This script will recompile python3.6 from source code and will install riscof"

apt-get install -y make build-essential libssl-dev zlib1g-dev \
libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev \
libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev \
libgdbm-dev libnss3-dev libedit-dev libc6-dev

wget https://www.python.org/ftp/python/3.6.15/Python-3.6.15.tgz
tar -xzf Python-3.6.15.tgz
rm Python-3.6.15.tgz
cd Python-3.6.15
./configure --enable-optimizations  -with-lto  --with-pydebug
make altinstall
python3.6 -m pip install git+https://github.com/riscv/riscof.git