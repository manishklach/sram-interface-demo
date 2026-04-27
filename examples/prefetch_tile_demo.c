#include "sram_mmio.h"
#include "sram_lowlevel.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define TILE_WORDS 32

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, 0x1000) != 0) return 1;

    uint32_t *tile = (uint32_t *)malloc(TILE_WORDS * sizeof(uint32_t));
    if (!tile) return 1;

    printf("[prefetch] preparing tile\n");
    for (int i = 0; i < TILE_WORDS; i++) {
        tile[i] = i;
    }

    printf("[prefetch] prefetching source lines\n");
    for (int i = 0; i < TILE_WORDS; i += 16) {
        sram_ll_prefetch_read(&tile[i]);
    }

    printf("[prefetch] ordered copy to SRAM aperture\n");
    sram_copy_to(&sram, 0, tile, TILE_WORDS * sizeof(uint32_t));

    uint32_t readback[TILE_WORDS];
    sram_copy_from(&sram, 0, readback, TILE_WORDS * sizeof(uint32_t));

    printf("[prefetch] verify ");
    if (memcmp(tile, readback, TILE_WORDS * sizeof(uint32_t)) == 0) {
        printf("OK ✔\n");
    } else {
        printf("FAILED ✘\n");
    }

    free(tile);
    sram_close(&sram);
    return 0;
}
