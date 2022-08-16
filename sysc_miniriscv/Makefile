SYSTEMC_ARCH = linux

OPT    = -O3
DEBUG  = -g
OTHER  = -Wall -Wno-deprecated
CXXFLAGS =  $(OTHER) $(DEBUG)
# CXXFLAGS = $(DEBUG) $(OTHER)

MODULE = run
SRCS = main.cpp $(wildcard miniriscv/src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY:	clean

include ./Makefile.defs

clean::
	rm -f *.vcd Makefile.deps
