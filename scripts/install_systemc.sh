cd /usr/local/
apt install build-essential make wget git gcc g++ automake
wget http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.gz
tar -xzf systemc-2.3.3.gz

if ( ! -d /usr/local/systemc-2.3.3/ )
then
    mkdir /usr/local/systemc-2.3.3/
fi
cd systemc-2.3.3 && mkdir -p objdir && cd objdir
../configure --prefix=/usr/local/systemc-2.3.3/
make
make install