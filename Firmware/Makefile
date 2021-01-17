MCU_TARGET = attiny2313
F_CPU = 500000	# CKSEL = 0010 (4 MHz), CKDIV8 programmed

CC = avr-gcc
OBJCOPY = avr-objcopy

override CFLAGS = -Wall -Wextra -Werror -Os -mmcu=$(MCU_TARGET) -DF_CPU=$(F_CPU)

all: main.hex

main.o: main.c

main.elf: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

main.hex: main.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

.PHONY: clean

clean:
	rm -f main.o main.elf main.hex
