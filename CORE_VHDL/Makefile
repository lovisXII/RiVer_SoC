BUILD=build
GHDL=ghdl 
COMPONENTS=core IFETCH/* DEC/dec EXE/* MEM/* WBK/* REG/* UTIL/* ram
TB=core_tb 
MODS=$(COMPONENTS) $(TB)
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addallsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))



all : analyse elaborate build 
	$(GHDL) -r $(TB) --vcd=core_tb.vcd 

build : 
	cd $(BUILD) && g++ ../core.cpp, `$(GHDL) --list-link $(TB)`

analyse : $(SOURCES)
	$(GHDL) -a -v $^ && $(GHDL) --bind $(TB)

elaborate : analyse 
	$(GHDL) -e -v $(TB) 	

clean : 
	rm *.vcd *.cf