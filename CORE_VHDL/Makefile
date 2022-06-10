BUILD=build
GHDLFLAGS?=--workdir=$(BUILD)
GHDL=$(ghdl)
COMPONENTS=core IFETCH/* DEC/dec EXE/* MEM/* WBK/* REG/* UTIL/* ram
TB=core_tb 
MODS=$(COMPONENTS) $(TB)
SOURCES=$(addsuffix .vhd, $(MODS))

all : analyse elaborate build 
	$(GHDL) -r $(TB) --vcd=core_tb.vcd 

build : 
	cd $(BUILD) && clang ../ram_sim.cpp -Wl, `$(GHDL) --list-link $(TB)`

analyse : $(SOURCES)
	$(GHDL) -a -v $^ && $(GHDL) --bind $(TB)

elaborate : analyse 
	$(GHDL) -e -v $(TB) 	

clean : 
	rm *.vcd *.cf *.o *.lst