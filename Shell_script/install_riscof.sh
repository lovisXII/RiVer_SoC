#! /bin/bash
# This script allow you to setup the riscof test frimework
# You will need it if you really want to test our design, otherwise it will be
# useless for you to run this script

export TEMPORARY_PATH=$PWD/../riscof
cd $TEMPORARY_PATH
################### PYTHON SETUP ################### 

echo "Installing python"
echo "If you are running on Ubuntu 22.04 you can have some issue installing riscof, if so please run install_python_ub_22_04.sh"

sudo add-apt-repository ppa:deadsnakes/ppa
sudo apt-get update
sudo apt-get install python3.6 -y
sudo apt install python3-pip
pip3 install --upgrade pip

################### RISCOF SETUP ################### 

if ! command -v riscof &> /dev/null 
then
    echo "Installing riscof"
    pip3 install git+https://github.com/riscv/riscof.git
fi


################### SPIKE SETUP ################### 

if ! command -v spike &> /dev/null
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
        sudo ../configure --prefix=/opt/
        sudo make -j4
        sudo make install #sudo is required depending on the path chosen in the previous setup
        echo "export PATH=/opt/bin:$PATH" >> ~/.bashrc
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
PATH=$PWD/../SIM/SystemC/CORE/core_tb">>config.ini

riscof --verbose info arch-test --clone
