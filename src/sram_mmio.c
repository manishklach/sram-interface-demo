#include "sram_mmio.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
int sram_open(sram_region_t *region, uintptr_t phys_addr, size_t size) {
    region->fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (region->fd < 0) return -1;
    region->base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, region->fd, phys_addr);
    if (region->base == MAP_FAILED) return -1;
    region->size = size;
    region->phys_addr = phys_addr;
    return 0;
}
void sram_close(sram_region_t *region) {
    munmap((void *)region->base, region->size);
    close(region->fd);
}
uint32_t sram_read32(sram_region_t *region, size_t offset) {
    return *(volatile uint32_t *)(region->base + offset);
}
void sram_write32(sram_region_t *region, size_t offset, uint32_t value) {
    *(volatile uint32_t *)(region->base + offset) = value;
}
int sram_copy_to(sram_region_t *region, size_t offset, const void *src, size_t len) {
    memcpy((void *)(region->base + offset), src, len);
    return 0;
}
int sram_copy_from(sram_region_t *region, size_t offset, void *dst, size_t len) {
    memcpy(dst, (const void *)(region->base + offset), len);
    return 0;
}
