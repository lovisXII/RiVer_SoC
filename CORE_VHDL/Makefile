BUILD=build
SIM_DIR=sim
RISC=riscv32-unknown-elf-gcc
TEST=test_add.s
GHDL?=/opt/ghdl/bin/ghdl
GHDLFLAGS?=-v --workdir=$(BUILD)
COMPONENTS=core IFETCH/ifetch DEC/dec EXE/shifter EXE/alu EXE/exec MEM/mem REG/reg WBK/wbk UTIL/fifo 
TB=core_tb
MODS=$(TB) $(COMPONENTS) 
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))
CC=gcc
LD_SCRIPT= -T kernel.ld
RISC_FLAGS= -nostdlib $(LD_SCRIPT)

all: run

run: core_tb exception.o
	cp app.ld exception.o sim/
	./$(TB) $(TEST) --vcd=$(SIM_DIR)/$(TB).vcd
	
core_tb : $(OBJ) $(BUILD)/ram_sim.o $(BUILD)/core_tb.o
	${GHDL} -e ${GHDLFLAGS} -Wl,$(BUILD)/ram_sim.o core_tb

$(BUILD)/ram_sim.o :
	gcc -c ram_sim.c -o $(BUILD)/ram_sim.o

$(BUILD)/a.out: $(BUILD)/e~$(TB).o ram_sim.c 
	cd $(BUILD) && $(CC) ../ram_sim.c -Wl,`$(GHDL) --list-link $(TB)`

$(BUILD)/e~$(TB).o: $(OBJ)
	cd $(BUILD) && $(GHDL) --bind $(TB)

$(BUILD)/core_tb.o: $(SOURCES)
	$(GHDL) -a $(GHDLFLAGS) $^

exception.o : exception.s 
	$(RISC) $(RISC_FLAGS) $^ -o $@


clean:
	rm $(BUILD)/* $(SIM_DIR)/* -f *.lst *.o *.cf core_tb *.vcd *.out *.out.txt
