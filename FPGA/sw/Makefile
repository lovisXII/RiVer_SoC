INCLUDE_PATH=./include 
SRC=main.c src/driver_wb.c src/driver_leds.c src/driver_lcd.c
CC=riscv32-unknown-elf-gcc
OBJCOPY=riscv32-unknown-elf-objcopy
CC_FLAGS=--std=gnu99 -Wl,-Bstatic,-T,app.lds,-Map,app.map,--strip-debug -lgcc -I ./include
OBJ=main.c driver_wb.c driver_leds.c driver_lcd.c
PYTHON=python3
COE_GEN=xilinx_coe_generator.py
MEM_SIZE=4096

all : coe	

coe : hex
	cd ./coe && $(PYTHON) $(COE_GEN)

hex : obj
	$(PYTHON) makehex.py main.bin $(MEM_SIZE) > ./coe/main.txt

obj : compile
	riscv32-unknown-elf-objcopy -O binary main.elf main.bin

compile : 
	riscv32-unknown-elf-gcc -Os -ffreestanding -nostdlib -march=rv32im -o main.elf $(SRC) $(CC_FLAGS)		 
clean : 
	rm -f *.o *.bin *.elf *.map *.hex