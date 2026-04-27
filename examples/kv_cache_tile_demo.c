#include "sram_mmio.h"
#include "mem_hint.h"
#include <stdio.h>
#include <string.h>

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, 0x10000) != 0) return 1;

    mem_hint_region_t hint;
    const char *data = "KV_BLOCK_7_LAYER_12";
    char buffer[64] = {0};

    printf("[1] reserve \"kv_tile_0\"\n");
    mem_hint_reserve(&hint, "kv_tile_0", MEM_TIER_SRAM, strlen(data) + 1, 0x0, 0);

    printf("[2] bind → SRAM\n");
    mem_hint_bind_to_sram(&sram, &hint);

    printf("[3] write → payload\n");
    mem_hint_write(&sram, &hint, data);

    printf("[4] readback → ");
    mem_hint_read(&sram, &hint, buffer);

    if (strcmp(data, buffer) == 0) {
        printf("verified ✔\n");
    } else {
        printf("failed ✘\n");
    }

    mem_hint_release(&hint);
    sram_close(&sram);
    return 0;
}
