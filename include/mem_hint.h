#ifndef MEM_HINT_H
#define MEM_HINT_H

#include <stddef.h>
#include <stdint.h>
#include "sram_mmio.h"

typedef enum {
    MEM_TIER_SRAM = 0,
    MEM_TIER_DRAM = 1,
    MEM_TIER_CXL  = 2,
    MEM_TIER_SSD  = 3,
    MEM_TIER_NVME = 4,
    MEM_TIER_UNKNOWN = 5
} mem_tier_t;

typedef struct {
    const char *name;
    mem_tier_t tier;
    size_t size;
    size_t offset;
    uint32_t flags;
    // Owner ID: could represent a layer, expert, thread, core, or accelerator queue
    uint32_t owner_id;
} mem_hint_region_t;

int mem_hint_reserve(mem_hint_region_t *hint,
                     const char *name,
                     mem_tier_t tier,
                     size_t size,
                     size_t offset);

int mem_hint_bind_to_sram(sram_region_t *sram,
                          const mem_hint_region_t *hint,
                          const void *src);

int mem_hint_read_from_sram(sram_region_t *sram,
                            const mem_hint_region_t *hint,
                            void *dst);

const char *mem_tier_name(mem_tier_t tier);

#endif
