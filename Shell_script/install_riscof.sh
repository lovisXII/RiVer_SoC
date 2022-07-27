#! /bin/bash
# This script allow you to setup the riscof test frimework
# You will need it if you really want to test our design, otherwise it will be
# useless for you to run this script

export TEMPORARY_PATH=$PWD/../riscof
cd $TEMPORARY_PATH
################### PYTHON SETUP ################### 

echo "Please run in sudo"
echo "Installing python"
echo "If you are running on Ubuntu 22.04 you can have some issue installing riscof, if so please run install_python_ub_22_04.sh"

<<<<<<< HEAD
# add-apt-repository ppa:deadsnakes/ppa
# apt-get update
# apt-get install python3.6 -y
=======
# sudo apt-get install python3.6
# pip3 install --upgrade pip
# sudo add-apt-repository ppa:deadsnakes/ppa
# sudo apt-get update
# sudo apt-get install python3.6 -y
>>>>>>> main
# pip3 install --upgrade pip


################### RISCOF SETUP ################### 

<<<<<<< HEAD
if ! command -v riscof &> /dev/null 
=======
if [ ! [ riscof > /dev/null 2>&1 ] ]
>>>>>>> main
then
    echo "Installing riscof"
    pip3 install git+https://github.com/riscv/riscof.git
fi


################### GNU-TOOLCHAIN SETUP ################### 

echo "Installing GNU Toolchain"

# sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
<<<<<<< HEAD
if ! command -v riscv32-unknown-elf-gcc &> /dev/null 
=======
if [ ! [ riscv32-unknown-elf-gcc -v > /dev/null 2>&1 ] ] 
>>>>>>> main
then 
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo " PLEASE BE PATIENT THIS OPERATION CAN BE QUITE LONG"
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo "Launching install_riscv to install riscv compiler" 
    sudo ./install_riscv.sh
else
    echo "gnu toolchain is already installed and setup in the bashrc"
fi

################### SPIKE SETUP ################### 

<<<<<<< HEAD
if ! command -v spike &> /dev/null
=======
if [ ! [ spike -v > /dev/null 2>&1 ] ] 
>>>>>>> main
then
        echo "Unlucky you, spike isn't installed : Installing spike"
        echo "#######################################" 
        echo "#######################################"
        echo "#######################################"
        echo "#######################################"
        echo " PLEASE BE PATIENT THIS OPERATION IS QUITE LONG"
        echo "#######################################" 
        echo "#######################################"
        echo "#######################################"
        echo "#######################################"
        sudo apt-get install device-tree-compiler
        cd /tmp/ && git clone https://github.com/riscv-software-src/riscv-isa-sim.git
        cd riscv-isa-sim
        mkdir build
        cd build
        ../configure --prefix=$TEMPORARY_PATH
        make -j4
        sudo make install #sudo is required depending on the path chosen in the previous setup
        echo "export PATH=/opt/spike/bin:$PATH" >> ~/.bashrc
        source ~/.bashrc
else 
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo "LUCKY YOU ! SPIKE IS ALREADY INSTALLED"
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
fi
<<<<<<< HEAD
=======


################### CONFIG SETUP ################### 

<<<<<<< HEAD
<<<<<<< HEAD
cd $TEMPORARY_PATH 
if ! [ -f "config.ini" ] 
then
    echo "
    [RISCOF]
    ReferencePlugin=spike
    ReferencePluginPath=$PWD/RISC-V-project/riscof/spike
    DUTPlugin=projet
    DUTPluginPath=$PWD/RISC-V-project/riscof/projet
>>>>>>> main


################### CONFIG SETUP ################### 

cd $TEMPORARY_PATH 
rm config.ini
echo "
[RISCOF]
ReferencePlugin=spike
ReferencePluginPath=$PWD/spike
DUTPlugin=projet
DUTPluginPath=$PWD/projet

[spike]
pluginpath=$PWD/spike
ispec=$PWD/spike/spike_isa.yaml
pspec=$PWD/spike/spike_platform.yaml
target_run=1

[sail_cSim]
pluginpath=$PWD/sail_cSim

[projet]
pluginpath=$PWD/projet
ispec=$PWD/projet/projet_isa.yaml
pspec=$PWD/projet/projet_platform.yaml
PATH=$PWD/../CORE/core_tb">>config.ini

<<<<<<< HEAD
=======
    [projet]
    pluginpath=$PWD/RISC-V-project/riscof/projet
    ispec=$PWD/RISC-V-project/riscof/projet/projet_isa.yaml
    pspec=$PWD/RISC-V-project/riscof/projet/projet_platform.yaml
    PATH=$PWD/RISC-V-project/CORE/core_tb">>config.ini
fi
=======
=======
>>>>>>> 6884fe6c (update before change branch)
cd $TEMPORARY_PATH/riscof
if [ -f config.ini ]
then 
    echo "config.ini exist, removing it"
    rm config.ini
fi
echo "Creating config.ini in $TEMPORARY_PATH/riscof"
echo "
[RISCOF]
ReferencePlugin=spike
ReferencePluginPath=$PWD/riscof/spike
DUTPlugin=projet
DUTPluginPath=$PWD/riscof/projet
[spike]
pluginpath=$PWD/riscof/spike
ispec=$PWD/riscof/spike/spike_isa.yaml
pspec=$PWD/riscof/spike/spike_platform.yaml
target_run=1
[sail_cSim]
pluginpath=$PWD/riscof/sail_cSim
[projet]
pluginpath=$PWD/riscof/projet
ispec=$PWD/riscof/projet/projet_isa.yaml
pspec=$PWD/riscof/projet/projet_platform.yaml
PATH=$PWD/CORE/core_tb">>config.ini 


<<<<<<< HEAD
>>>>>>> 6884fe6c (update before change branch)
=======
>>>>>>> 6884fe6c (update before change branch)
>>>>>>> main
riscof --verbose info arch-test --clone