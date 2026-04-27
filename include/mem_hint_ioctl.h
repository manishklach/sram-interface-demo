#ifndef MEM_HINT_IOCTL_H
#define MEM_HINT_IOCTL_H

#include <stdint.h>

/**
 * Conceptual IOCTL interface for a future /dev/mem_hint driver.
 * 
 * NOTE: This is a design sketch for documentation purposes. 
 * These commands are not yet implemented in a kernel driver.
 */

/* Memory Tiers */
#define MEM_TIER_DRAM 0
#define MEM_TIER_SRAM 1
#define MEM_TIER_CXL  2

/* Conceptual IOCTL command placeholders */
#define MEM_HINT_IOC_RESERVE 0xC001
#define MEM_HINT_IOC_BIND    0xC002
#define MEM_HINT_IOC_PROMOTE 0xC003
#define MEM_HINT_IOC_EVICT   0xC004
#define MEM_HINT_IOC_QUERY   0xC005

struct mem_hint_ioc_reserve {
    char     name[64];   /* User-defined tag for the region */
    uint64_t size;       /* Requested size in bytes */
    uint32_t tier;       /* Target memory tier (e.g., SRAM) */
    uint32_t flags;      /* Allocation flags (alignment, persistence) */
    uint32_t owner_id;   /* Process or context ID */
};

struct mem_hint_ioc_bind {
    uint64_t region_id;  /* ID returned by reserve */
    uint64_t offset;     /* Offset within the tier aperture */
    uint64_t size;       /* Slice size to bind */
};

struct mem_hint_ioc_query {
    uint64_t region_id;
    uint64_t size;       /* Actual resident size */
    uint32_t tier;       /* Current tier (might have migrated) */
    uint32_t state;      /* residency state (0=evicted, 1=resident) */
};

#endif
