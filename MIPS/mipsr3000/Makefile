CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS)

IFETCH=ifetch.o mux_ifetch.o ff_ifetch.o
DECOD=decode.o mux_decode.o ff_decode.o decode_rom.o
EXEC=execute.o mux_execute.o ff_execute.o
MEM=memory.o mux_memory.o ff_memory.o
WBK=writeback.o mux_writeback.o ff_writeback.o
MISC=misc.o mux_misc.o ff_misc.o
CACHE=i_cache.cpp rom.cpp ram.cpp
OTHER=alu.o regfile.o forwardunit.o pipeunit.o pi_default.cpp pi_control.cpp mips_dec.cpp

all: mips

mips:  main.cpp $(IFETCH) $(DECOD) $(EXEC) $(MEM) $(WBK) $(MISC) $(CACHE) $(OTHER)
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp
	$(CC) -c $^ $(C_ARGS)

clean: 
	rm -f *.o main alu_tb shifter_tb
