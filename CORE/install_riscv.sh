echo"##################################################################################################"
echo "This script need to be in sudo mode otherwise you will get an error at the end of the downloading"
echo"##################################################################################################"
wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
mkdir -p /opt/riscv
tar -xzf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C /opt/riscv/
OS="`uname`"
if grep -q microsoft /proc/version; then
  echo "Ubuntu on Windows"
  echo "export PATH=$PATH:/opt/riscv/bin" >> ~/.bashrcrisc
  source ~/.bashrcrisc
  . ~/.bashrcrisc
else
  echo "Native Linux"
  echo "export PATH=$PATH:/opt/riscv/bin" >> ~/.bashrc
  source ~/.bashrc
  . ~/.bashrc
fi
rm -f riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz