#include "sram_arch.h"

const char *sram_arch_name(void) {
#if defined(__x86_64__)
    return "x86_64";
#elif defined(__aarch64__)
    return "aarch64";
#else
    return "unknown";
#endif
}

const char *sram_arch_barrier_name(void) {
#if defined(__x86_64__)
    return "mfence";
#elif defined(__aarch64__)
    return "dmb sy";
#else
    return "__sync_synchronize";
#endif
}

void sram_arch_compiler_barrier(void) {
    /* Prevents compiler reordering. */
    asm volatile("" ::: "memory");
}

void sram_arch_barrier(void) {
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

uint32_t sram_arch_read32(volatile void *addr) {
    uint32_t val;
    sram_arch_barrier();
    val = *(volatile uint32_t *)addr;
    sram_arch_barrier();
    return val;
}

void sram_arch_write32(volatile void *addr, uint32_t value) {
    sram_arch_barrier();
    *(volatile uint32_t *)addr = value;
    sram_arch_barrier();
}

void sram_arch_copy32_to_sram(volatile void *dst, const uint32_t *src, size_t words) {
    volatile uint32_t *d = (volatile uint32_t *)dst;
    for (size_t i = 0; i < words; i++) {
        sram_arch_write32(&d[i], src[i]);
    }
}

void sram_arch_copy32_from_sram(uint32_t *dst, volatile void *src, size_t words) {
    volatile uint32_t *s = (volatile uint32_t *)src;
    for (size_t i = 0; i < words; i++) {
        dst[i] = sram_arch_read32(&s[i]);
    }
}
