SYSTEMC?=/usr/local/systemc-2.3.3/
CC=g++
INCLUDE_PATH=-I$(SYSTEMC)/include
LINK_PATH=-L $(SYSTEMC)lib-linux64
LINKER_ARGS=-rpath=$(SYSTEMC)lib-linux64 
LIBS=-lsystemc -lm
C_ARGS=$(INCLUDE_PATH) $(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
OBJECTS=test_buffer.o

all: test_buffer

test_buffer: $(OBJECTS)  buffer.h
	$(CC) $(OBJECTS) -o $@ $(C_ARGS) 

$(OBJECTS): test_buffer.cpp buffer.h 
	$(CC) -c $< $(C_ARGS) -o $@

clean: 
	rm -f *.o test_buffer *.vcd $(OBJECTS)