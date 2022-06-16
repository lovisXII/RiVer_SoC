CFLAGS = -Wall -g
INCLUDE_DIR = ../include

all : ReadElf

ElfObj.o : ElfObj.c
	gcc ${CFLAGS} -I${INCLUDE_DIR} -c ElfObj.c
	cp ElfObj.o ../lib/

ReadElf.o : ReadElf.c
	gcc ${CFLAGS} -I${INCLUDE_DIR} -c ReadElf.c
	cp ReadElf.o ../lib/

ReadElf : ElfObj.o ReadElf.o
	gcc ${CFLAGS} -o ReadElf ElfObj.o ReadElf.o
	cp ReadElf ../bin/

clean :
	rm  -f *.o ReadElf ../lib/*.o
