#include "sram_mmio.h"
#include "mem_hint.h"
#include <stdio.h>
#include <string.h>

#define DEMO_SRAM_SIZE 0x10000

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, DEMO_SRAM_SIZE) != 0) {
        fprintf(stderr, "Failed to open mock SRAM\n");
        return 1;
    }

    printf("=== KV-Cache Tile Residency Demo ===\n\n");

    const char *kv_payload = "KV_TILE_HEAD_7_TOKEN_1492";
    char readback[64] = {0};

    mem_hint_region_t kv_hint;
    kv_hint.flags = 0;
    kv_hint.owner_id = 7; // Head 7

    printf("[1] Reserving SRAM for KV Tile...\n");
    mem_hint_reserve(&kv_hint, "kv_tile_h7_t1492", MEM_TIER_SRAM, strlen(kv_payload) + 1, 0x1000);

    printf("\n[2] Binding KV Tile to SRAM...\n");
    mem_hint_bind_to_sram(&sram, &kv_hint, kv_payload);

    printf("\n[3] Reading back KV Tile from SRAM...\n");
    mem_hint_read_from_sram(&sram, &kv_hint, readback);

    printf("\n[4] Verification: ");
    if (strcmp(kv_payload, readback) == 0) {
        printf("SUCCESS! ('%s')\n", readback);
    } else {
        printf("FAILED! Expected '%s', got '%s'\n", kv_payload, readback);
    }

    sram_close(&sram);
    return 0;
}
