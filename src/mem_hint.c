#include "mem_hint.h"

#include <stdio.h>

const char *mem_tier_name(mem_tier_t tier) {
    switch (tier) {
        case MEM_TIER_SRAM: return "SRAM";
        case MEM_TIER_DRAM: return "DRAM";
        case MEM_TIER_CXL:  return "CXL";
        case MEM_TIER_SSD:  return "SSD";
        case MEM_TIER_NVME: return "NVMe";
        case MEM_TIER_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

int mem_hint_reserve(mem_hint_region_t *hint,
                     const char *name,
                     mem_tier_t tier,
                     size_t size,
                     size_t offset) {
    if (!hint || !name || size == 0) {
        return -1;
    }

    hint->name = name;
    hint->preferred_tier = tier;
    hint->size = size;
    hint->offset = offset;
    hint->flags = 0;
    hint->owner_id = 0;

    printf("[mem_hint] reserve name=%s tier=%s size=%zu offset=0x%zx\n",
           hint->name,
           mem_tier_name(hint->preferred_tier),
           hint->size,
           hint->offset);

    return 0;
}

int mem_hint_bind_to_sram(sram_region_t *sram,
                          const mem_hint_region_t *hint,
                          const void *src) {
    if (!sram || !hint || !src) {
        return -1;
    }

    printf("[mem_hint] bind %s to SRAM at offset=0x%zx size=%zu\n",
           hint->name,
           hint->offset,
           hint->size);

    return sram_copy_to(sram, hint->offset, src, hint->size);
}

int mem_hint_read_from_sram(sram_region_t *sram,
                            const mem_hint_region_t *hint,
                            void *dst) {
    if (!sram || !hint || !dst) {
        return -1;
    }

    printf("[mem_hint] read %s from SRAM at offset=0x%zx size=%zu\n",
           hint->name,
           hint->offset,
           hint->size);

    return sram_copy_from(sram, hint->offset, dst, hint->size);
}
