SYSTEMC?=/usr/local/systemc-2.3.3/
CC=g++
RISC=riscv32-unknown-elf-gcc 
INCLUDE_PATH=-I$(SYSTEMC)/include -I../ELFIO
LINK_PATH=-L $(SYSTEMC)/lib-linux64
LINKER_ARGS=-rpath=$(SYSTEMC)/lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=SoC_tb.o CORE/core.o CORE/IFETCH/ifetch.o CORE/DEC/dec.o CORE/EXE/alu.o \
CORE/EXE/shifter.o CORE/EXE/x0_multiplier.o CORE/EXE/Diviseur.o CORE/EXE/exec.o CORE/MEM/mem.o CORE/MEM/x1_multiplier.o \
CORE/REG/reg.o CORE/WBK/wbk.o CORE/WBK/x2_multiplier.o CORE/CSR/csr.o CORE/CACHES/icache.o \
CORE/CACHES/dcache.o CORE/CACHES/buffercache.o CORE/TIMER/timer.o Wishbone/wb_bus.o Wishbone/wb_arbiter.o \
Wishbone/IP_RIVER.o Wishbone/wb_river_mc.o Wishbone/IP_RAM.o Wishbone/wb_ram_sc.o Wishbone/RAM.o Wishbone/Multiplexor.o
LD_SCRIPT= -T CORE/kernel.ld
RISC_FLAGS= -nostdlib $(LD_SCRIPT)

all: SoC_tb

SoC_tb: $(OBJECTS) kernel 
	$(CC) $(OBJECTS) -o $@ $(C_ARGS) 

kernel: SW/exception.s SW/reset.s
	$(RISC) $(RISC_FLAGS) $^ -o $@
	
%.o: %.cpp  %.h
	$(CC) -c $< $(C_ARGS) -o $@

%.o: %.cpp 
	$(CC) -c $< $(C_ARGS) -o $@

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd $(OBJECTS) core_tb SoC_tb a.out
	rm kernel
