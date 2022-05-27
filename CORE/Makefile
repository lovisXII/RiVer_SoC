CC=g++
RISC=riscv32-unknown-elf-gcc 
INCLUDE_PATH=-I/usr/local/systemc-2.3.3/include -I../ELFIO 
LINK_PATH=-L /usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=core.o core_tb.o IFETCH/ifetch.o DEC/dec.o EXE/alu.o EXE/shifter.o EXE/exec.o MEM/mem.o REG/reg.o WBK/wbk.o CSR/csr.o
LD_SCRIPT= -T seg.ld
RISC_FLAGS= -nostdlib $(LD_SCRIPT)

all: core_tb

core_tb: $(OBJECTS) kernel
	$(CC) $(OBJECTS) -o $@ $(C_ARGS) 

kernel: exception.s reset.s
	$(RISC) $(RISC_FLAGS) $^ -o $@
	
%.o: %.cpp  %.h
	$(CC) -c $< $(C_ARGS) -o $@

%.o: %.cpp 
	$(CC) -c $< $(C_ARGS) -o $@

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd $(OBJECTS) core_tb a.out
	rm kernel