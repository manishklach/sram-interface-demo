#ifndef SRAM_ARCH_H
#define SRAM_ARCH_H

#include <stdint.h>
#include <stddef.h>

/**
 * Architecture identification
 */
const char *sram_arch_name(void);
const char *sram_arch_barrier_name(void);

/**
 * Barriers
 */
void sram_arch_compiler_barrier(void);
void sram_arch_barrier(void);

/**
 * Atomic 32-bit ordered access
 */
uint32_t sram_arch_read32(volatile void *addr);
void sram_arch_write32(volatile void *addr, uint32_t value);

/**
 * Ordered copy helpers (word-based)
 */
void sram_arch_copy32_to_sram(volatile void *dst, const uint32_t *src, size_t words);
void sram_arch_copy32_from_sram(uint32_t *dst, volatile void *src, size_t words);

#endif
