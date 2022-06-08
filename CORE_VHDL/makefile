GHDL=ghdl 
COMPONENTS=IFETCH/* DEC/dec EXE/* MEM/* WBK/* REG/* UTIL/*
CORE=core
TB=core_tb 
MODS=$(COMPONENTS) $(CORE) $(TB)
SOURCES=$(addsuffix .vhd, $(MODS))

all : analyse elaborate
	$(GHDL) -r $(TB) --vcd=core_tb.vcd

analyse : $(SOURCES)
	$(GHDL) -a -v $^

elaborate : analyse 
	$(GHDL) -e -v $(TB) 	

clean : 
	rm *.vcd *.cf