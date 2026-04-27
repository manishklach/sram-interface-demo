CC=gcc
CFLAGS=-O2 -Wall -Iinclude
all:
  src/demo.c src/sram_mmio.c -o sram_demo
clean:
rm -f sram_demo
