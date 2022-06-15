BUILD=build
SIM_DIR=sim
GHDL?=/opt/ghdl/bin/ghdl
GHDLFLAGS?=-v --workdir=$(BUILD)
COMPONENTS=core IFETCH/ifetch DEC/dec EXE/shifter EXE/alu EXE/exec MEM/mem REG/reg WBK/wbk UTIL/fifo 
TB=core_tb
MODS=$(TB) $(COMPONENTS) 
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))
CC=gcc


all: core_tb

view: run
	gtkwave $(SIM_DIR)/$(TB).vcd

run: all 
	cd $(SIM_DIR) && ../$(BUILD)/a.out ../test_add.s --vcd=$(TB).vcd

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

clean:
	rm $(BUILD)/* -f *.lst *.o *.cf core_tb
