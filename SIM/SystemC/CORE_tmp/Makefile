SYSTEMC?=/usr/local/systemc-2.3.3/
CC=g++
RISC=riscv32-unknown-elf-gcc 
INCLUDE_PATH=-I$(SYSTEMC)/include -I../../ELFIO
LINK_PATH=-L $(SYSTEMC)/lib-linux64
LINKER_ARGS=-rpath=$(SYSTEMC)/lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=core.o core_tb.o IFETCH/ifetch.o DEC/dec.o EXE/alu.o \
EXE/shifter.o EXE/x0_multiplier.o EXE/Diviseur.o EXE/exec.o MEM/mem.o MEM/x1_multiplier.o \
REG/reg.o WBK/wbk.o WBK/x2_multiplier.o CSR/csr.o TIMER/timer.o
SW_DIR=../../../SOFT/
LD_SCRIPT= -T $(SW_DIR)/kernel.ld
RISC_FLAGS= -nostdlib $(LD_SCRIPT)
all: core_tb

core_tb: $(OBJECTS) kernel
	$(CC) $(OBJECTS) -o $@ $(C_ARGS) 

kernel: $(SW_DIR)/exception.s $(SW_DIR)/reset.s
	$(RISC) $(RISC_FLAGS) $^ -o $@
	
%.o: %.cpp  %.h
	$(CC) -c $< $(C_ARGS) -o $@

%.o: %.cpp 
	$(CC) -c $< $(C_ARGS) -o $@

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd $(OBJECTS) core_tb a.out \
	kernel *.out.txt *.out.txt.s