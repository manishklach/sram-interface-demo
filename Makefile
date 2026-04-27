CC=gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude
SRC_LIBS=src/sram_mmio.c src/mem_hint.c src/sram_arch.c src/sram_lowlevel.c
OUT=sram_demo

all: $(OUT) examples

$(OUT): src/demo.c $(SRC_LIBS) include/sram_mmio.h include/mem_hint.h include/sram_arch.h include/sram_lowlevel.h
	$(CC) $(CFLAGS) src/demo.c $(SRC_LIBS) -o $(OUT)

examples: kv_cache_tile_demo tensor_tile_demo ordered_tile_copy_demo latency_bench cacheline_flush_demo prefetch_tile_demo tlb_page_walk_notes_demo

kv_cache_tile_demo: examples/kv_cache_tile_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/kv_cache_tile_demo.c $(SRC_LIBS) -o kv_cache_tile_demo

tensor_tile_demo: examples/tensor_tile_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/tensor_tile_demo.c $(SRC_LIBS) -o tensor_tile_demo

ordered_tile_copy_demo: examples/ordered_tile_copy_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/ordered_tile_copy_demo.c $(SRC_LIBS) -o ordered_tile_copy_demo

latency_bench: examples/latency_bench.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/latency_bench.c $(SRC_LIBS) -o latency_bench

cacheline_flush_demo: examples/cacheline_flush_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/cacheline_flush_demo.c $(SRC_LIBS) -o cacheline_flush_demo

prefetch_tile_demo: examples/prefetch_tile_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/prefetch_tile_demo.c $(SRC_LIBS) -o prefetch_tile_demo

tlb_page_walk_notes_demo: examples/tlb_page_walk_notes_demo.c $(SRC_LIBS)
	$(CC) $(CFLAGS) examples/tlb_page_walk_notes_demo.c $(SRC_LIBS) -o tlb_page_walk_notes_demo

demo: $(OUT)

test: all
	./sram_demo
	./kv_cache_tile_demo
	./tensor_tile_demo
	./ordered_tile_copy_demo
	./latency_bench 10000
	./cacheline_flush_demo
	./prefetch_tile_demo
	./tlb_page_walk_notes_demo

clean:
	rm -f $(OUT) kv_cache_tile_demo tensor_tile_demo ordered_tile_copy_demo latency_bench cacheline_flush_demo prefetch_tile_demo tlb_page_walk_notes_demo *.exe *.o
