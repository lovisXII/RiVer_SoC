CC=g++
C_args= -g -W

all: risc_sim


run:
	./risc_sim
risc_sim: risc_sim.o main.o
	$(CC) $^ -o $@ $(C_args)

%.o : %.cpp
	$(CC) -c $^ $(C_args)

clean:
	rm -f *.o risc_sim