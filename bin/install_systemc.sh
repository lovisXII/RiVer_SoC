#This script allow you to install systemc on your system
OS="`uname`"

if [ "$1" = "clean" ]
then    
    echo "systemc-2.3.3 and rm systemc-2.3.3.gz"
    rm -r systemc-2.3.3 && rm systemc-2.3.3.gz
else
    if [ "$OS" = Linux ]
    then
        echo "Installing systemc for Linux distribution"
        apt install build-essential make wget git gcc g++ automake
        wget http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.gz
        echo "Uncompressing the tar archive into current directory"
        export CXX=g++
        tar -xzf systemc-2.3.3.gz
        
        if [ ! -d /usr/local/systemc-2.3.3/ ]
        then
            mkdir /usr/local/systemc-2.3.3/
        fi
        cd systemc-2.3.3 && mkdir -p objdir && cd objdir
        ../configure
        ../configure --prefix=/usr/local/systemc-2.3.3/
        make -j$(nproc)
        make install
    fi
fi
