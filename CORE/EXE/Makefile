CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS)


#exec_tb : exec_tb.o exec.o	
#	$(CC) $^ -o $@ $(C_ARGS)
all: alu_tb shifter_tb

alu_tb: alu.o alu_tb.o
	$(CC) $^ -o $@ $(C_ARGS)

shifter_tb: shifter.o shifter_tb.o
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp
	$(CC) -c $^ $(C_ARGS)

clean: 
	rm -f *.o main alu_tb shifter_tb