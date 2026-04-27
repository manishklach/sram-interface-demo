#include "sram_arch.h"

uint32_t sram_arch_read32(volatile void *addr) {
    /* Volatile C access ensures the compiler does not optimize this away. */
    return *(volatile uint32_t *)addr;
}

void sram_arch_write32(volatile void *addr, uint32_t value) {
    /* Volatile C access ensures the compiler does not optimize this away. */
    *(volatile uint32_t *)addr = value;
}

void sram_arch_memory_barrier(void) {
#if defined(__x86_64__)
    /* x86-64 MFENCE: Serializes load and store operations. */
    asm volatile("mfence" ::: "memory");
#elif defined(__aarch64__)
    /* ARMv8-A DMB SY: Data Memory Barrier for full system. */
    asm volatile("dmb sy" ::: "memory");
#else
    /* Portable GCC builtin: issues a full memory barrier. */
    __sync_synchronize();
#endif
}
