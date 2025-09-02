# c compiler and flag. if is mac os, use clang else gcc
ifeq ($(shell uname), Darwin)
	CC = clang
else
	CC = gcc
endif
CFLAGS = -Wall -std=c89


all:
	$(CC) $(CFLAGS) -Ihelper -Iinclude -o nes_emulator helper/debug_printf.c src/main.c src/Bus.c src/CPU.c
	./nes_emulator


test_runner:
	$(CC) $(CFLAGS) -Ihelper -Iinclude -Itest -o test_output helper/debug_printf.c test/test_runner.c test/test_bus.c test/test_cpu.c test/unity.c src/Bus.c src/CPU.c
	./test_output