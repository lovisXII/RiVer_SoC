echo "Please be in sudo"
echo "This Script setup a ws/Ubuntu envirronment"
apt update
apt install ghdl gtkwave
apt install gedit
apt-get install python3.6
apt install gcc
apt install clang
apt install ssh
apt install git
# echo "installing compiler for pi 4"
# wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.70.tar.gz
# tar zxvf bcm2835-1.70.tar.gz
# cd bcm2835-1.70
# ./configure
# make
# sudo make check
# sudo make install
sh install_riscv.sh
sh install_systemc.sh