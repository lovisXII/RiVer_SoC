BUILD=build
GHDL?=/opt/ghdl/bin/ghdl
GHDLFLAGS?=--workdir=$(BUILD)
COMPONENTS=core IFETCH/ifetch DEC/dec EXE/shifter EXE/alu EXE/exec MEM/mem REG/reg UTIL/fifo 
TB=core_tb
MODS=$(TB) $(COMPONENTS) 
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))
CC=clang

all: $(BUILD)/a.out 

analyse: $(OBJ)

$(BUILD)/a.out: $(BUILD)/e~$(TB).o ram_sim.c
	cd $(BUILD) && $(CC) ../ram_sim.c -Wl,`$(GHDL) --list-link $(TB)`

$(BUILD)/e~$(TB).o: $(OBJ)
	cd $(BUILD) && $(GHDL) --bind $(TB)
	
$(BUILD)/core_tb.o: $(SOURCES)
	$(GHDL) -a $(GHDLFLAGS) $^

clean:
	rm $(BUILD)/* -f *.lst *.o *.cf
	