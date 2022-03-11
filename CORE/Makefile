CC=g++
INCLUDE_PATH=-I/usr/local/systemc-2.3.3/include -I../ELFIO 
LINK_PATH=-L /usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=core.o core_tb.o IFETCH/ifetch.o DEC/dec.o EXE/alu.o EXE/shifter.o EXE/exec.o MEM/mem.o REG/reg.o WBK/wbk.o

all: core_tb

core_tb: $(OBJECTS)
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp  %.h
	$(CC) -c $< $(C_ARGS) -o $@

%.o: %.cpp 
	$(CC) -c $< $(C_ARGS) -o $@

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd $(OBJECTS) core_tb a.out