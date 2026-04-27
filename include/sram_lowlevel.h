#ifndef SRAM_LOWLEVEL_H
#define SRAM_LOWLEVEL_H

#include <stdint.h>
#include <stddef.h>

/**
 * Architecture Identification
 */
const char *sram_ll_arch_name(void);

/**
 * Barriers
 */
void sram_ll_compiler_barrier(void);
void sram_ll_full_barrier(void);
void sram_ll_pause(void);

/**
 * Timing (Architecture Specific)
 * Returns a high-resolution cycle counter or monotonic nanoseconds as fallback.
 */
uint64_t sram_ll_rdtsc(void);

/**
 * Cache Maintenance (Hints)
 */
void sram_ll_clflush(const void *addr);
void sram_ll_prefetch_read(const void *addr);
void sram_ll_prefetch_write(const void *addr);

#endif
