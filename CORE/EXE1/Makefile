CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g


all: exec.o

%.o: %.cpp
	$(CC) -c $^ $(C_ARGS) 

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd
	cd ../UTIL/fifo_78b/ && make clean