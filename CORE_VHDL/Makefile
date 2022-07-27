BUILD=build
SIM_DIR=sim
RISC=riscv32-unknown-elf-gcc
TEST=test_add.s
GHDL?=/opt/ghdl/bin/ghdl
GHDLFLAGS?=-v --workdir=$(BUILD)
COMPONENTS=UTIL/fifo UTIL/util UTIL/csa core IFETCH/ifetch DEC/dec EXE/shifter EXE/alu EXE/exec EXE/x0_multiplier EXE/divider MEM/mem MEM/x1_multiplier REG/reg WBK/wbk WBK/x2_multiplier CSR/csr 
TB=core_tb
MODS=$(TB) $(COMPONENTS) 
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))
CC=gcc
SW_DIR=../SW
LD_SCRIPT= -T $(SW_DIR)/kernel.ld
RISC_FLAGS= -nostdlib $(LD_SCRIPT)

all: run		

run: core_tb kernel

	cp ../SW/app.ld kernel sim/
	./$(TB) $(TEST) --vcd=$(SIM_DIR)/$(TB).vcd --wave=$(SIM_DIR)/$(TB).ghw

core_tb : warning $(OBJ) $(BUILD)/ram_sim.o $(BUILD)/core_tb.o 
	${GHDL} -e ${GHDLFLAGS} -Wl,$(BUILD)/ram_sim.o core_tb

$(BUILD)/ram_sim.o :
	gcc -c ram_sim.c -o $(BUILD)/ram_sim.o

$(BUILD)/a.out: $(BUILD)/e~$(TB).o ram_sim.c 
	cd $(BUILD) && $(CC) ../ram_sim.c -Wl,`$(GHDL) --list-link $(TB)`

$(BUILD)/e~$(TB).o: $(OBJ)
	cd $(BUILD) && $(GHDL) --bind $(TB)

$(BUILD)/core_tb.o: $(SOURCES)
	$(GHDL) -a $(GHDLFLAGS) $^

kernel: $(SW_DIR)/exception.s $(SW_DIR)/reset.s
	$(RISC) $(RISC_FLAGS) $^ -o $@

warning :
	@echo
	@echo Please dont forget to give an argument when doing make, otherwise it could segfault
	@echo You have to do : make TEST=pth_to_the_test/test_name
	@echo
clean:
	rm $(BUILD)/* $(SIM_DIR)/* -f *.lst *.o *.cf core_tb *.vcd *.ghw *.out \
	*.out.txt *.out.txt.s
