CC=g++
INCLUDE_PATH=-I/usr/local/systemc-2.3.3/include -I../ELFIO/elfio
LINK_PATH=-L /usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=IFETCH/ifetch.o DEC/dec.o EXE/alu.o EXE/shifter.o EXE/exec.o EXE/exec_tb.o MEM/mem.o REG/reg.o WBK/wbk.o UTIL/fifo_78b/fifo_78b.o UTIL/fifo_74b/fifo_74b.o UTIL/fifo_32b/fifo_32b.o UTIL/fifo_41b/fifo_41b.o UTIL/fifo_110b/fifo_110b.o

all: alu_tb shifter_tb exec_tb

core_tb: $(OBJECTS)
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp
	$(CC) -c $^ $(C_ARGS) -o $@

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd