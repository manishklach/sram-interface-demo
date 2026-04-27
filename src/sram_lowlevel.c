#include "sram_lowlevel.h"
#include <time.h>

const char *sram_ll_arch_name(void) {
#if defined(__x86_64__)
    return "x86_64";
#elif defined(__aarch64__)
    return "aarch64";
#else
    return "unknown";
#endif
}

void sram_ll_compiler_barrier(void) {
    asm volatile("" ::: "memory");
}

void sram_ll_full_barrier(void) {
#if defined(__x86_64__)
    asm volatile("mfence" ::: "memory");
#elif defined(__aarch64__)
    asm volatile("dmb sy" ::: "memory");
#else
    __sync_synchronize();
#endif
}

void sram_ll_pause(void) {
#if defined(__x86_64__)
    asm volatile("pause");
#elif defined(__aarch64__)
    asm volatile("yield");
#else
    /* No-op */
#endif
}

uint64_t sram_ll_rdtsc(void) {
#if defined(__x86_64__)
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#elif defined(__aarch64__)
    uint64_t v;
    asm volatile("mrs %0, cntvct_el0" : "=r"(v));
    return v;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
}

void sram_ll_clflush(const void *addr) {
#if defined(__x86_64__)
    asm volatile("clflush (%0)" :: "r"(addr) : "memory");
#else
    (void)addr;
#endif
}

void sram_ll_prefetch_read(const void *addr) {
#if defined(__x86_64__)
    asm volatile("prefetcht0 (%0)" :: "r"(addr));
#elif defined(__aarch64__)
    asm volatile("prfm pldl1keep, [%0]" :: "r"(addr));
#else
    (void)addr;
#endif
}

void sram_ll_prefetch_write(const void *addr) {
#if defined(__x86_64__)
    asm volatile("prefetchw (%0)" :: "r"(addr));
#elif defined(__aarch64__)
    asm volatile("prfm pstl1keep, [%0]" :: "r"(addr));
#else
    (void)addr;
#endif
}
