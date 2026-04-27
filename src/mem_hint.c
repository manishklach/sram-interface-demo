#include "mem_hint.h"
#include <string.h>

const char* mem_tier_name(mem_tier_t tier) {
    switch (tier) {
        case MEM_TIER_SRAM:    return "SRAM";
        case MEM_TIER_DRAM:    return "DRAM";
        case MEM_TIER_CXL:     return "CXL";
        case MEM_TIER_NVME:    return "NVME";
        case MEM_TIER_UNKNOWN: return "UNKNOWN";
        default:               return "UNKNOWN";
    }
}

int mem_hint_reserve(mem_hint_region_t *hint,
                     const char *name,
                     mem_tier_t tier,
                     size_t size,
                     size_t offset,
                     size_t alignment) {
    if (!hint || !name || size == 0) {
        return MEM_HINT_ERR_INVALID;
    }

    // Alignment validation
    if (alignment > 0 && (offset % alignment) != 0) {
        return MEM_HINT_ERR_ALIGN;
    }

    strncpy(hint->name, name, sizeof(hint->name) - 1);
    hint->name[sizeof(hint->name) - 1] = '\0';
    hint->tier = tier;
    hint->size = size;
    hint->offset = offset;
    hint->alignment = alignment;
    hint->flags = 0;
    hint->owner_id = 0;

    return MEM_HINT_SUCCESS;
}

int mem_hint_bind_to_sram(sram_region_t *sram, const mem_hint_region_t *hint) {
    if (!sram || !hint) {
        return MEM_HINT_ERR_INVALID;
    }

    // Validation logic
    if (hint->offset + hint->size > sram->size) {
        return MEM_HINT_ERR_OOB;
    }

    return MEM_HINT_SUCCESS;
}

int mem_hint_write(sram_region_t *sram, const mem_hint_region_t *hint, const void *src) {
    if (!sram || !hint || !src) {
        return MEM_HINT_ERR_INVALID;
    }
    if (sram_copy_to(sram, hint->offset, src, hint->size) != 0) {
        return MEM_HINT_ERR_BIND;
    }
    return MEM_HINT_SUCCESS;
}

int mem_hint_read(sram_region_t *sram, const mem_hint_region_t *hint, void *dst) {
    if (!sram || !hint || !dst) {
        return MEM_HINT_ERR_INVALID;
    }
    if (sram_copy_from(sram, hint->offset, dst, hint->size) != 0) {
        return MEM_HINT_ERR_BIND;
    }
    return MEM_HINT_SUCCESS;
}

void mem_hint_release(mem_hint_region_t *hint) {
    if (!hint) return;
    memset(hint, 0, sizeof(mem_hint_region_t));
}
