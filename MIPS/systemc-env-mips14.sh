#!/bin/bash
########################################################################
# systemc-env.sh: Set up the environment for SystemC
########################################################################
# Author:  Torsten MAEHNE <Torsten.Maehne@lip6.fr>
# Created: 2014-01-30
########################################################################
#
# This shell script prepares the shell environment for building or
# using the SystemC libraries. You can either source it from the command
# line or in your .bashrc:
#
# . ./systemc-env.sh [--build-version=2.2|2.3.0|2.3.1]
#                    [--build-type=Debug|Release] \
#                    [-h|--help] \
#                    [-v|--version] \
#
########################################################################


########################################################################
# Print help message
########################################################################
function systemc_environment_help_message() {
    cat <<EOF
This script should be sourced to prepare your environment for SystemC:
. $0 [--build-version=2.2|2.3.0|2.3.1] [--build-type=Debug|Release]
EOF
}


########################################################################
# Print version message
########################################################################
function systemc_environment_version_message() {
    cat <<EOF
systemc-env.sh: Set up the environment for SystemC
Author:  Torsten MAEHNE <Torsten.Maehne@lip6.fr>
Version: unreleased
EOF
}


########################################################################
# Function to manipulate the path variable
########################################################################
path_append ()  { path_remove $1; export PATH="$PATH:$1"; }
path_prepend () { path_remove $1; export PATH="$1:$PATH"; }
path_remove ()  { export PATH=`echo -n $PATH | awk -v RS=: -v ORS=: '$0 != "'$1'"' | sed 's/:$//'`; }


# Default SystemC version (either 2.2, 2.3.0, or 2.3.1)
SYSTEMC_VERSION=2.3.1
# Default SystemC build type (either Release or Debug)
SYSTEMC_BUILD_TYPE=Release
# Process the command line arguments
for i in $*
do
    case $i in
        --build-version=*)
            SYSTEMC_VERSION=${i#*=}
            ;;
    	--build-type=*)
	    SYSTEMC_BUILD_TYPE=${i#*=}
	    ;;
        -h | --help)
            systemc_environment_help_message
            exit 0
            ;;
        -v | --version)
            systemc_environment_version_message
            exit 0
            ;;
    	*)
            # unknown option
            echo "Error: Unknown option \"$i\"" 1>&2
            exit 1
            ;;
    esac
done

case $SYSTEMC_BUILD_TYPE in
    Debug)
        SYSTEMC_BUILD_SUFFIX=dbg
    ;;
    Release)
        SYSTEMC_BUILD_SUFFIX=rel
    ;;
    *)
        echo "Error: Unknown build type \"$BUILD_TYPE\". Use either \"Debug\” or \"Release\"." 1>&2
esac

case $SYSTEMC_VERSION in
#    2.2)
#        export BOOST_ROOT=/opt/systemc-2.2-$SYSTEMC_BUILD_SUFFIX
#        export SYSTEMC_ARCH=linux
#        export SYSTEMC_ROOT=/opt/systemc-2.2-$SYSTEMC_BUILD_SUFFIX
#        export SYSTEMC_HOME=$SYSTEMC_ROOT
#        export SYSTEMC_AMS_HOME=$SYSTEMC_ROOT
#        export SYSTEMC_INCLUDE_DIRS=-I$SYSTEMC_ROOT/include
#        export SCV_HOME=$SYSTEMC_ROOT
#        export TLM_HOME=$SYSTEMC_ROOT
#        export SYSTEMC_LIBRARY_DIRS="-Wl,-rpath=$SYSTEMC_ROOT/lib -L$SYSTEMC_ROOT/lib"
#        export SYSTEMC_ARCH=linux
#        export PATH=$PATH:/opt/soclib/utils/bin:/opt/cxtools/install/bin
#        #export LD_LIBRARY_PATH=$SYSTEMC_ROOT
#        ;;
#    2.3.0)
#        export BOOST_ROOT=/usr
#        export SYSTEMC_ARCH=linux
#        export SYSTEMC_ROOT=/opt/systemc-2.3.0-$SYSTEMC_BUILD_SUFFIX
#        export SYSTEMC_HOME=$SYSTEMC_ROOT
#        export SYSTEMC_AMS_HOME=/opt/systemc-ams-2.0alpha1-$SYSTEMC_BUILD_SUFFIX
#        export SYSTEMC_INCLUDE_DIRS="-I$SYSTEMC_ROOT/include -I$SYSTEMC_AMS_HOME/include"
#        #export SCV_HOME=$SYSTEMC_ROOT
#        export TLM_HOME=$SYSTEMC_ROOT
#        export SYSTEMC_LIBRARY_DIRS="-Wl,-rpath=$SYSTEMC_HOME/lib-$SYSTEMC_ARCH -L$SYSTEMC_HOME/lib-$SYSTEMC_ARCH -Wl,-rpath=$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH -L$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH"
#        export PATH=$PATH:/opt/soclib/utils/bin:/opt/cxtools/install/bin
#        #export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-$SYSTEMC_ARCH:$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH
#        ;;
    2.3.1)
        export BOOST_ROOT=/usr
        export SYSTEMC_ARCH=linux64
        export TARGET_ARCH=
        #export SYSTEMC_MDVP_HOME=/dsk/l1/misc/genius/systemc_mdvp-1.0.0
        export SYSTEMC_ROOT=/users/outil/systemcams/systemc-2.3.1 
        export SYSTEMC_HOME=$SYSTEMC_ROOT
        export SYSTEMC_AMS_HOME=/users/outil/systemcams/systemc-ams-2.1
        export SYSTEMC_INCLUDE_DIRS="-I$SYSTEMC_ROOT/include -I$SYSTEMC_AMS_HOME/include"
        export SCV_HOME=$SYSTEMC_ROOT
        export TLM_HOME=$SYSTEMC_ROOT
        export SYSTEMC_LIBRARY_DIRS="-Wl,-rpath=$SYSTEMC_HOME/lib-$SYSTEMC_ARCH -L$SYSTEMC_HOME/lib-$SYSTEMC_ARCH -Wl,-rpath=$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH -L$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH"
        #export PATH=$PATH:/dsk/l1/misc/genius/soclib/utils/bin:/dsk/l1/misc/genius/mipsel-unknown-elf/bin
	#export PATH=$PATH:/dsk/l1/misc/genius/soclib/utils/bin:/dsk/l1/misc/genius/mipsel-unknown-elf/bin
	export PATH=$PATH:/opt/soclib/utils/bin:/opt/mipsel-unknown-elf/bin
        #export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-$SYSTEMC_ARCH:$SYSTEMC_AMS_HOME/lib-$SYSTEMC_ARCH
        ;;
    *)
        echo "Error: Unknown SystemC version \"$SYSTEMC_VERSION\". Use either \"2.2\”, \"2.3.0\", or  \"2.3.1\"." 1>&2
        ;;
esac

export SYSTEMC=$SYSTEMC_HOME
export TLM2=$TLM_HOME
export SYSTEMC_AMS=$SYSTEMC_AMS_HOME
# export SYSTEMC_AMS_PATH=$SYSTEMC_AMS_HOME
# export SYSTEMCAMS=$SYSTEMC_AMS_HOME
# export SYSTEMCAMS_HOME=$SYSTEMC_AMS_HOME
# export SYSTEMCAMS_PATH=$SYSTEMC_AMS_HOME

#export TARGET_ARCH=$SYSTEMC_ARCH
