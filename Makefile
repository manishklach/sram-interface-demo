CC=gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude

SRC=src/demo.c src/sram_mmio.c src/mem_hint.c src/sram_arch.c
OUT=sram_demo

all: $(OUT) examples

$(OUT): $(SRC) include/sram_mmio.h include/mem_hint.h
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

examples: kv_cache_tile_demo tensor_tile_demo

kv_cache_tile_demo: examples/kv_cache_tile_demo.c src/sram_mmio.c src/mem_hint.c src/sram_arch.c include/sram_mmio.h include/mem_hint.h include/sram_arch.h
	$(CC) $(CFLAGS) examples/kv_cache_tile_demo.c src/sram_mmio.c src/mem_hint.c src/sram_arch.c -o kv_cache_tile_demo

tensor_tile_demo: examples/tensor_tile_demo.c src/sram_mmio.c src/mem_hint.c src/sram_arch.c include/sram_mmio.h include/mem_hint.h include/sram_arch.h
	$(CC) $(CFLAGS) examples/tensor_tile_demo.c src/sram_mmio.c src/mem_hint.c src/sram_arch.c -o tensor_tile_demo

demo: $(OUT)

test: all
	./sram_demo
	./kv_cache_tile_demo
	./tensor_tile_demo

clean:
	rm -f $(OUT) kv_cache_tile_demo tensor_tile_demo *.exe
