SYSTEMC?=/usr/local/systemc-2.3.3/
CC=g++
RISC=riscv32-unknown-elf-gcc 
INCLUDE_PATH=-I$(SYSTEMC)/include -I../../ELFIO
LINK_PATH=-L $(SYSTEMC)/lib-linux64
LINKER_ARGS=-rpath=$(SYSTEMC)/lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=core.o core_tb.o IFETCH/ifetch.o DEC/dec.o EXE1/x0_multiplier.o EXE1/exec_s1.o \
EXE2/exec_s2.o MEM1/mem_s1.o MEM1/x1_multiplier.o MEM2/mem_s2.o REG/reg.o \
WBK1/wbk_s1.o WBK1/x2_multiplier.o WBK2/wbk_s2.o CSR/csr.o \
CACHES/icache.o CACHES/dcache.o CACHES/buffercache.o
SW_DIR=../SW
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
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd $(OBJECTS) core_tb a.out *.out.txt *.out.txt.s
	rm kernel.o 