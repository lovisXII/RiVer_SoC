C_FLAGS = -Wall -g
INCLUDE_DIR = ../include
ELF_INCLUDE = ../ReadElf/include

all : test_decod arm_sim arm_ghdl.o

decod.o : decod.c
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -c decod.c
	cp decod.o ../lib/

arm3.o : arm3.c
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -c arm3.c
	cp arm3.o ../lib/

mem.o : mem.c
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -I${ELF_INCLUDE} -c mem.c
	cp mem.o ../lib/

arm_sim.o : arm_sim.c
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -I${ELF_INCLUDE} -c arm_sim.c
	cp arm_sim.o ../lib/

arm_ghdl.o : arm_ghdl.c mem.o arm3.o
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -I${ELF_INCLUDE} -c arm_ghdl.c
	cp arm_ghdl.o ../lib/

arm_sim : decod.o arm_sim.o mem.o arm3.o
	gcc ${C_FLAGS} -o arm_sim decod.o arm_sim.o ../ReadElf/lib/ElfObj.o mem.o arm3.o
	cp arm_sim ../bin/

test_decod.o : test_decod.c
	gcc ${C_FLAGS} -I${INCLUDE_DIR} -c test_decod.c

test_decod : decod.o test_decod.o
	gcc ${C_FLAGS} -o test_decod decod.o test_decod.o

clean :
	rm -f *.o test_decod arm_sim ../lib/*.o
