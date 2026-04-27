#include "sram_mmio.h"
#include "mem_hint.h"
#include <stdio.h>
#include <stdint.h>

#define DEMO_SRAM_SIZE 0x10000
#define TILE_ELEMENTS 16

int main(void) {
    sram_region_t sram;
    if (sram_open_mock(&sram, DEMO_SRAM_SIZE) != 0) {
        fprintf(stderr, "Failed to open mock SRAM\n");
        return 1;
    }

    printf("=== Tensor Tile Staging Demo ===\n\n");

    float tensor_in[TILE_ELEMENTS];
    float tensor_out[TILE_ELEMENTS];

    for (int i = 0; i < TILE_ELEMENTS; i++) {
        tensor_in[i] = (float)(i * 1.5);
        tensor_out[i] = 0.0f;
    }

    mem_hint_region_t tensor_hint;
    tensor_hint.flags = 1; // 1 = Read/Write
    tensor_hint.owner_id = 42; // Example layer ID

    printf("[1] Reserving SRAM for Tensor Tile (size: %zu bytes)...\n", sizeof(tensor_in));
    mem_hint_reserve(&tensor_hint, "tensor_tile_L42", MEM_TIER_SRAM, sizeof(tensor_in), 0x2000);

    printf("\n[2] Binding Tensor Tile to SRAM...\n");
    mem_hint_bind_to_sram(&sram, &tensor_hint, tensor_in);

    printf("\n[3] Reading back Tensor Tile from SRAM...\n");
    mem_hint_read_from_sram(&sram, &tensor_hint, tensor_out);

    printf("\n[4] Verification:\n");
    int success = 1;
    for (int i = 0; i < TILE_ELEMENTS; i++) {
        if (tensor_in[i] != tensor_out[i]) {
            success = 0;
            printf("Mismatch at index %d: expected %f, got %f\n", i, tensor_in[i], tensor_out[i]);
        }
    }

    if (success) {
        printf("SUCCESS! All %d elements match.\n", TILE_ELEMENTS);
    } else {
        printf("FAILED!\n");
    }

    sram_close(&sram);
    return 0;
}
