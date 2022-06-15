BUILD=build
SIM_DIR=sim
GHDL?=/opt/ghdl/bin/ghdl
GHDLFLAGS?=--workdir=$(BUILD)
COMPONENTS=core IFETCH/ifetch DEC/dec EXE/shifter EXE/alu EXE/exec MEM/mem REG/reg WBK/wbk UTIL/fifo 
TB=core_tb
C_MOD_DIR = ../C_model/
MODS=$(TB) $(COMPONENTS) 
SOURCES=$(addsuffix .vhd, $(MODS))
OBJ=$(addsuffix .o, $(addprefix $(BUILD)/, $(notdir $(MODS))))
CC=gcc


all: core_tb

view: run
	gtkwave $(SIM_DIR)/$(TB).vcd

run: all 
	cd $(SIM_DIR) && ../$(BUILD)/a.out ../test_add.s --vcd=$(TB).vcd

# Building main tb

core_tb : $(OBJ) ${C_MOD_DIR}/ReadElf/lib/ElfObj.o $(BUILD)/ram_sim.o
	${GHDL} -e ${GHDLFLAGS} -Wl,$(BUILD)/ram_sim.o core_tb

$(BUILD)/ram_sim.o :
	gcc -c ram_sim.c -o $(BUILD)/ram_sim.o

analyse: $(OBJ)

# Interface with C 

$(BUILD)/a.out: $(BUILD)/e~$(TB).o ram_sim.c 
	cd $(BUILD) && $(CC) ../ram_sim.c -Wl,`$(GHDL) --list-link $(TB)`

$(BUILD)/e~$(TB).o: $(OBJ)
	cd $(BUILD) && $(GHDL) --bind $(TB)
# End of C Interface

# Building core_tb.o 

$(BUILD)/core_tb.o: $(SOURCES)
	$(GHDL) -a $(GHDLFLAGS) $^

# Building object file library

# ${C_MOD_DIR}/ReadElf/lib/ElfObj.o:
# 	make -C ${C_MOD_DIR}/ReadElf/src

clean:
	rm $(BUILD)/* -f *.lst *.o *.cf
	# make -C ${C_MOD_DIR}/ReadElf/src clean

# /opt/ghdl/bin/ghdl -e -v --workdir=../work -Wl,../C_model//lib/mem.o -Wl,../C_model//lib/arm_ghdl.o -Wl,../C_model//ReadElf/lib/ElfObj.o main_tb
# /opt/ghdl/bin/ghdl1-llvm --workdir=../work -P/opt/ghdl/lib/ghdl/ieee/v93/ -P/opt/ghdl/lib/ghdl/ --elab main_tb -l e~main_tb.lst -fpic -c -o e~main_tb.o e~main_tb
# /usr/bin/cc -o main_tb e~main_tb.o /opt/ghdl/lib/ghdl/std/v93/std_standard.o /opt/ghdl/lib/ghdl/ieee/v93/std_logic_1164.o /opt/ghdl/lib/ghdl/ieee/v93/std_logic_1164-body.o /opt/ghdl/lib/ghdl/ieee/v93/numeric_std.o /opt/ghdl/lib/ghdl/ieee/v93/numeric_std-body.o ../work/ram.o ../work/main_tb.o ../work/icache.o ../work/dcache.o ../work/core.o ../work/ifetch.o ../work/fifo_32b.o ../work/decod.o ../work/reg.o ../work/one_hot.o ../work/fifo_127b.o ../work/exec.o ../work/shifter.o ../work/shift_left.o ../work/shift_right.o ../work/ror.o ../work/alu.o ../work/fifo_72b.o ../work/mem.o ../C_model//lib/mem.o ../C_model//lib/arm_ghdl.o ../C_model//ReadElf/lib/ElfObj.o /opt/ghdl/lib/ghdl/libgrt.a -ldl -lm -L./ -lz -ldl -Wl,--version-script=/opt/ghdl/lib/ghdl/grt.ver -Wl,--export-dynamic
