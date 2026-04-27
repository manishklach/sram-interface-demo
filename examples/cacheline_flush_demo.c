#include "sram_lowlevel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALIGNMENT 64

int main(void) {
    void *ptr = NULL;
    /* Allocate cache-line aligned buffer */
    if (posix_memalign(&ptr, ALIGNMENT, ALIGNMENT) != 0) {
        return 1;
    }

    printf("[cache] arch: %s\n", sram_ll_arch_name());
    printf("[cache] writing cache line\n");
    
    volatile uint64_t *val = (volatile uint64_t *)ptr;
    *val = 0xDEADBEEFCAFEBABEULL;

    sram_ll_full_barrier();

    printf("[cache] clflush issued\n");
    sram_ll_clflush(ptr);
    sram_ll_full_barrier();

    if (*val == 0xDEADBEEFCAFEBABEULL) {
        printf("[cache] readback OK ✔\n");
    } else {
        printf("[cache] readback FAILED ✘\n");
    }

    printf("[cache] note: this is a cache-behavior demo, not SRAM latency\n");

    free(ptr);
    return 0;
}
