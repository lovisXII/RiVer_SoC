GHDL=ghdl
TB=../sim/tb_dec
COMPONENTS=dec ../UTIL/fifo
MODS= $(TB) $(COMPONENTS)
SOURCES=$(addsuffix .vhd, $(MODS))

all : analyse elaborate
	$(GHDL) -r $(TB) --vcd=$(word 1, $(COMPONENTS)).vcd

analyse : $(SOURCES)
	$(GHDL) -a $^

elaborate : analyse
	$(GHDL) -e $(TB)

clean :
	rm *.vcd *.cf