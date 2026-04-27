#ifndef SRAM_MMIO_H
#define SRAM_MMIO_H
#include <stdint.h>
#include <stddef.h>
typedef struct {
    int fd;
    volatile uint8_t *base;
    size_t size;
    uintptr_t phys_addr;
} sram_region_t;
int sram_open(sram_region_t *region, uintptr_t phys_addr, size_t size);
void sram_close(sram_region_t *region);
uint32_t sram_read32(sram_region_t *region, size_t offset);
void sram_write32(sram_region_t *region, size_t offset, uint32_t value);
int sram_copy_to(sram_region_t *region, size_t offset, const void *src, size_t len);
int sram_copy_from(sram_region_t *region, size_t offset, void *dst, size_t len);
#endif
