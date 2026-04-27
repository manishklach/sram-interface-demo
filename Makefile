CC=gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude

SRC=src/demo.c src/sram_mmio.c src/mem_hint.c
OUT=sram_demo

all: $(OUT)

$(OUT): $(SRC) include/sram_mmio.h include/mem_hint.h
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT) *.exe
