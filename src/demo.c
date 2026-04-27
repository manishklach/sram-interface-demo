#include "sram_mmio.h"
#include "mem_hint.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_SRAM_PHYS_ADDR 0x40000000
#define DEFAULT_SRAM_SIZE      0x10000

static int use_devmem(int argc, char **argv) {
    return argc >= 2 && strcmp(argv[1], "--devmem") == 0;
}

int main(int argc, char **argv) {
    sram_region_t sram;
    int rc;

    if (use_devmem(argc, argv)) {
        uintptr_t addr = DEFAULT_SRAM_PHYS_ADDR;
        size_t size = DEFAULT_SRAM_SIZE;

        if (argc >= 3) {
            addr = strtoull(argv[2], NULL, 0);
        }

        if (argc >= 4) {
            size = strtoull(argv[3], NULL, 0);
        }

        printf("Opening real SRAM via /dev/mem addr=0x%lx size=0x%lx\n",
               (unsigned long)addr,
               (unsigned long)size);

        rc = sram_open_devmem(&sram, addr, size);
    } else {
        printf("Opening mock SRAM backend size=0x%x\n", DEFAULT_SRAM_SIZE);
        rc = sram_open_mock(&sram, DEFAULT_SRAM_SIZE);
    }

    if (rc != 0) {
        fprintf(stderr, "Failed to open SRAM backend\n");
        return 1;
    }

    printf("\n=== Raw SRAM MMIO-style API ===\n");
    sram_write32(&sram, 0x00, 0xDEADBEEF);
    sram_write32(&sram, 0x04, 0xCAFEBABE);

    printf("SRAM[0x00] = 0x%08X\n", sram_read32(&sram, 0x00));
    printf("SRAM[0x04] = 0x%08X\n", sram_read32(&sram, 0x04));

    printf("\n=== Memory Hint / Residency API Demo ===\n");

    const char kv_tile[] = "KV_CACHE_TILE: token=42 layer=8 head=3";
    char readback[128] = {0};

    mem_hint_region_t kv_hint;
    mem_hint_reserve(&kv_hint,
                     "kv_cache_tile_L8_H3",
                     MEM_TIER_SRAM,
                     sizeof(kv_tile),
                     0x100);

    mem_hint_bind_to_sram(&sram, &kv_hint, kv_tile);
    mem_hint_read_from_sram(&sram, &kv_hint, readback);

    printf("Readback payload: %s\n", readback);

    sram_close(&sram);
    return 0;
}
