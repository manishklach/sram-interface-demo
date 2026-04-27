#include "sram_mmio.h"
#include "mem_hint.h"
#include <stdio.h>

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, 0x10000) != 0) return 1;

    mem_hint_region_t hint;
    float tensor[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    float readback[4] = {0};

    printf("[1] reserve \"tensor_tile\"\n");
    mem_hint_reserve(&hint, "tensor_tile", MEM_TIER_SRAM, sizeof(tensor), 0x500, 16);

    printf("[2] bind → SRAM\n");
    mem_hint_bind_to_sram(&sram, &hint);

    printf("[3] write → tensor\n");
    mem_hint_write(&sram, &hint, tensor);

    printf("[4] readback → ");
    mem_hint_read(&sram, &hint, readback);

    int ok = 1;
    for(int i=0; i<4; i++) if(tensor[i] != readback[i]) ok = 0;

    if (ok) printf("verified ✔\n");
    else printf("failed ✘\n");

    mem_hint_release(&hint);
    sram_close(&sram);
    return 0;
}
