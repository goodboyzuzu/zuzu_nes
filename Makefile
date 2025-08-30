# c compiler and flag. if is mac os, use clang else gcc
ifeq($(shell uname), Darwin)
	CC = clang
else
	CC = gcc
CFLAGS = -Wall -Wextra -std=c89

INCLUDE_PATH = -Iinc

all: nes_emulator
nes_emulator: src/main.c src/Bus.c src/CPU.c 
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o nes_emulator src/main.c src/Bus.c src/CPU.c