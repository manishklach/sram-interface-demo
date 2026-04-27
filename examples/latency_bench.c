#define _POSIX_C_SOURCE 199309L
#include "sram_mmio.h"
#include "sram_arch.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DEFAULT_ITERATIONS 1000000

static double get_nanos(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) * 1e9 + (double)(end.tv_nsec - start.tv_nsec);
}

int main(int argc, char **argv) {
    sram_region_t sram;
    long iterations = DEFAULT_ITERATIONS;

    if (argc >= 2) {
        iterations = atol(argv[1]);
        if (iterations <= 0) iterations = DEFAULT_ITERATIONS;
    }

    if (sram_open_mock(&sram, 0x1000) != 0) {
        fprintf(stderr, "Failed to open mock SRAM\n");
        return 1;
    }

    printf("[bench] backend: mock SRAM\n");
    printf("[bench] iterations: %ld\n", iterations);
    printf("[bench] note: this measures userspace/API overhead, not hardware latency\n");

    struct timespec start, end;
    uint32_t val = 0;

    // Write Benchmark
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long i = 0; i < iterations; i++) {
        sram_write32(&sram, 0, (uint32_t)i);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double write_total = get_nanos(start, end);

    // Read Benchmark
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long i = 0; i < iterations; i++) {
        val = sram_read32(&sram, 0);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double read_total = get_nanos(start, end);

    printf("[bench] write32 avg: %.2f ns/op\n", write_total / iterations);
    printf("[bench] read32 avg: %.2f ns/op\n", read_total / iterations);

    // Verify
    if (val == (uint32_t)(iterations - 1)) {
        printf("[bench] verify: OK ✔\n");
    } else {
        printf("[bench] verify: FAILED ✘ (expected %u, got %u)\n", (uint32_t)(iterations - 1), val);
    }

    sram_close(&sram);
    return 0;
}
