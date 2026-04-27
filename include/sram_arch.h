#ifndef SRAM_ARCH_H
#define SRAM_ARCH_H

#include <stdint.h>

/**
 * sram_arch_read32: Perform a 32-bit volatile read.
 */
uint32_t sram_arch_read32(volatile void *addr);

/**
 * sram_arch_write32: Perform a 32-bit volatile write.
 */
void sram_arch_write32(volatile void *addr, uint32_t value);

/**
 * sram_arch_memory_barrier: Ensure memory operation ordering.
 * 
 * Architecture-specific implementation of a full system-wide 
 * memory barrier where supported.
 */
void sram_arch_memory_barrier(void);

#endif
