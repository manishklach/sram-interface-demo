#include "sram_mmio.h"
#include "mem_hint.h"
#include "sram_arch.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_SRAM_PHYS_ADDR 0x40000000
#define DEFAULT_SRAM_SIZE      0x10000

static void print_help(const char *prog_name) {
    printf("SRAM Interface Demo (Memory Control Plane Prototype)\n\n");
    printf("Usage:\n");
    printf("  %s [options]\n\n", prog_name);
    printf("Options:\n");
    printf("  --help                        Show this help message\n");
    printf("  (no args)                     Run in safe mock mode (default)\n");
    printf("  --devmem <addr> <size>        Run in real hardware mode via /dev/mem\n\n");
    printf("WARNING: /dev/mem requires root and can crash your system.\n");
    printf("Examples:\n");
    printf("  %s\n", prog_name);
    printf("  sudo %s --devmem 0x40000000 0x10000\n", prog_name);
}

static int use_devmem(int argc, char **argv) {
    return argc >= 2 && strcmp(argv[1], "--devmem") == 0;
}

int main(int argc, char **argv) {
    sram_region_t sram;
    int rc;

    printf("[arch] ordered MMIO helpers enabled\n");
    printf("[arch] barrier = %s on %s\n", sram_arch_barrier_name(), sram_arch_name());

    if (argc >= 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_help(argv[0]);
        return 0;
    }

    if (use_devmem(argc, argv)) {
        if (argc < 4) {
            fprintf(stderr, "Error: --devmem requires <addr> and <size>\n");
            print_help(argv[0]);
            return 1;
        }
        uintptr_t addr = strtoull(argv[2], NULL, 0);
        size_t size = strtoull(argv[3], NULL, 0);
        rc = sram_open_devmem(&sram, addr, size);
    } else {
        rc = sram_open_mock(&sram, DEFAULT_SRAM_SIZE);
    }

    if (rc != 0) {
        fprintf(stderr, "Failed to open SRAM backend\n");
        return 1;
    }

    // Example Trace
    mem_hint_region_t kv_hint;
    size_t payload_size = 4096;
    uint8_t *payload = (uint8_t *)malloc(payload_size);
    uint8_t *readback = (uint8_t *)malloc(payload_size);
    
    if (!payload || !readback) return 1;
    memset(payload, 0x42, payload_size);
    memset(readback, 0, payload_size);

    printf("[mem_hint] reserve \"kv_tile_0\"\n");
    mem_hint_reserve(&kv_hint, "kv_tile_0", MEM_TIER_SRAM, payload_size, 0x100, 16);

    printf("[mem_hint] bind → SRAM offset 0x%zx\n", kv_hint.offset);
    if (mem_hint_bind_to_sram(&sram, &kv_hint) != 0) {
        fprintf(stderr, "Bind failed\n");
        free(payload);
        free(readback);
        goto cleanup;
    }

    printf("[mem_hint] write → %zu bytes\n", kv_hint.size);
    mem_hint_write(&sram, &kv_hint, payload);

    printf("[mem_hint] readback → ");
    mem_hint_read(&sram, &kv_hint, readback);

    if (memcmp(payload, readback, payload_size) == 0) {
        printf("verified ✔\n");
    } else {
        printf("failed ✘\n");
    }

    free(payload);
    free(readback);

cleanup:
    mem_hint_release(&kv_hint);
    sram_close(&sram);
    return 0;
}
