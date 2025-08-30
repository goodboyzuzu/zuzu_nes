# c compiler and flag. if is mac os, use clang else gcc
ifeq ($(shell uname), Darwin)
	CC = clang
else
	CC = gcc
endif
CFLAGS = -Wall -std=c99

INCLUDE_PATH = -Iinclude

all:
	$(CC) $(CFLAGS) -Iinclude -o nes_emulator src/main.c src/Bus.c src/CPU.c
	./nes_emulator


test_runner:
	$(CC) $(CFLAGS) -Iinclude -Itest -o test_output test/test_runner.c test/unity.c src/Bus.c src/CPU.c
	./test_output