CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_FLAGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS)

fifo_41b: fifo_41b.o fifo_41b_tb.o
	$(CC) $^ -o $@ $(C_FLAGS)

%.o: %.cpp
	$(CC) -c $^ $(C_FLAGS) 

clean:
	rm -f *.o fifo_41b
