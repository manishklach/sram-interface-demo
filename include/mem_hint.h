#ifndef MEM_HINT_H
#define MEM_HINT_H

#include <stddef.h>
#include <stdint.h>
#include "sram_mmio.h"

typedef enum {
    MEM_TIER_SRAM,
    MEM_TIER_DRAM,
    MEM_TIER_CXL,
    MEM_TIER_NVME,
    MEM_TIER_UNKNOWN
} mem_tier_t;

/**
 * mem_hint_region_t: A simplified model of a future memory control plane region.
 */
typedef struct {
    char name[64];
    mem_tier_t tier;
    size_t size;
    size_t offset;
    uint32_t flags;
    int owner_id;  // layer / expert / thread / core / queue
} mem_hint_region_t;

/**
 * Reserve a memory region with a hint.
 */
int mem_hint_reserve(mem_hint_region_t *hint,
                     const char *name,
                     mem_tier_t tier,
                     size_t size,
                     size_t offset);

/**
 * Bind a reserved region to a specific SRAM backend.
 */
int mem_hint_bind_to_sram(sram_region_t *sram, const mem_hint_region_t *hint);

/**
 * Write data to the bound SRAM region.
 */
int mem_hint_write(sram_region_t *sram, const mem_hint_region_t *hint, const void *src);

/**
 * Read data from the bound SRAM region.
 */
int mem_hint_read(sram_region_t *sram, const mem_hint_region_t *hint, void *dst);

/**
 * Release the reservation.
 */
void mem_hint_release(mem_hint_region_t *hint);

/**
 * Helper to get a human-readable name for a memory tier.
 */
const char* mem_tier_name(mem_tier_t tier);

#endif
