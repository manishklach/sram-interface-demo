#include "sram_mmio.h"
#include "sram_arch.h"
#include <stdio.h>
#include <stdint.h>

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, 0x1000) != 0) return 1;

    uint32_t tile[16];
    uint32_t readback[16];
    
    printf("[demo] preparing 16-word tile\n");
    for (int i = 0; i < 16; i++) {
        tile[i] = 0xDEADBEEF + i;
    }

    printf("[demo] ordered copy to SRAM aperture\n");
    /* This uses the architecture-specific word-based copy loop */
    sram_arch_copy32_to_sram((volatile void *)sram.base, tile, 16);

    printf("[demo] ordered readback\n");
    /* This uses the architecture-specific word-based readback loop */
    sram_arch_copy32_from_sram(readback, (volatile void *)sram.base, 16);

    printf("[demo] verify ");
    int ok = 1;
    for (int i = 0; i < 16; i++) {
        if (tile[i] != readback[i]) {
            ok = 0;
            break;
        }
    }

    if (ok) printf("OK ✔\n");
    else printf("FAILED ✘\n");

    sram_close(&sram);
    return 0;
}
