CC=gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude
SRC_LIBS=src/sram_mmio.c src/mem_hint.c src/sram_arch.c
OUT=sram_demo

all: $(OUT) examples

$(OUT): src/demo.c $(SRC_LIBS) include/sram_mmio.h include/mem_hint.h include/sram_arch.h
	$(CC) $(CFLAGS) src/demo.c $(SRC_LIBS) -o $(OUT)

examples: kv_cache_tile_demo tensor_tile_demo ordered_tile_copy_demo latency_bench

kv_cache_tile_demo: examples/kv_cache_tile_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/kv_cache_tile_demo.c $(SRC_LIBS) -o kv_cache_tile_demo

tensor_tile_demo: examples/tensor_tile_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/tensor_tile_demo.c $(SRC_LIBS) -o tensor_tile_demo

ordered_tile_copy_demo: examples/ordered_tile_copy_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/ordered_tile_copy_demo.c $(SRC_LIBS) -o ordered_tile_copy_demo

latency_bench: examples/latency_bench.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/latency_bench.c $(SRC_LIBS) -o latency_bench

demo: $(OUT)

test: all
	./sram_demo
	./kv_cache_tile_demo
	./tensor_tile_demo
	./ordered_tile_copy_demo
	./latency_bench 10000

clean:
	rm -f $(OUT) kv_cache_tile_demo tensor_tile_demo ordered_tile_copy_demo latency_bench *.exe *.o
