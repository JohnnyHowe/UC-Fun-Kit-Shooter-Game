# File:   Makefile
# Authors:Jonathon Howe, Tomoya Sakai
# Date:   10 Oct 2019
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/pio.h player.h communication.h ../../utils/font.h ../../utils/tinygl.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

player.o: player.c player.h game_display.h ../../drivers/navswitch.h shot.h ../../drivers/avr/ir_uart.h communication.h
	$(CC) -c $(CFLAGS) $< -o $@

shot.o: shot.c shot.h game_display.h
	$(CC) -c $(CFLAGS) $< -o $@

game_display.o: game_display.c game_display.h ../../drivers/avr/pio.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../utils/pacer.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

communication.o: communication.c communication.h ../../drivers/avr/ir_uart.h shot.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.aaa
game.out: game.o system.o pio.o navswitch.o player.o game_display.o shot.o pacer.o timer.o communication.o ir_uart.o timer0.o prescale.o usart1.o font.o tinygl.o display.o ledmat.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


