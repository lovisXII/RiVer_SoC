#!/bin/bash 
echo "This script will allow you to run our project"

if (riscv32-unknown-elf-gcc -v > /dev/null 2>&1) then
    echo "riscv compiler is already installed"
else 
    echo "riscv compiler is not setup, installing now"
    cd Test_called && sudo ./install_riscv.sh 
fi

echo "Our project is divise in 3 major section :
1. You can run our systemC implementation
2. You can run our VHDL implementation
3. You can run riscof on both implementation
4. If you want to setup 1,2,3"
echo "Please enter the number corresponding to what you want to install"

while : ; do
    read -n 1 k <&1

    if [[ $k = 1 ]] ; then
        cd Test_called/ && sudo ./install_systemc.sh
        break
    elif [[ $k = 2 ]] ; then
        cd Test_called/ && sudo ./install_ghdl.sh        
        break
    elif [[ $k = 3 ]] ; then    
        cd Test_called/ && sudo ./install_riscof.sh    
        break
    elif [[ $k = 4 ]] ; then
        cd Test_called/ && sudo ./install_systemc.sh
        ./install_ghdl
        ./install_riscof
        break    
    fi
done