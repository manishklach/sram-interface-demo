#include "sram_mmio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#endif

int sram_open_mock(sram_region_t *region, size_t size) {
    if (!region || size == 0) {
        return -1;
    }

    uint8_t *buf = (uint8_t *)calloc(size, 1);
    if (!buf) {
        return -1;
    }

    region->fd = -1;
    region->base = buf;
    region->size = size;
    region->phys_addr = 0;
    region->backend = SRAM_BACKEND_MOCK;

    return 0;
}

int sram_open_devmem(sram_region_t *region, uintptr_t phys_addr, size_t size) {
#ifndef __linux__
    (void)region;
    (void)phys_addr;
    (void)size;
    return -1;
#else
    if (!region || size == 0) {
        return -1;
    }

    region->fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (region->fd < 0) {
        return -1;
    }

    region->base = mmap(
        NULL,
        size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        region->fd,
        phys_addr
    );

    if (region->base == MAP_FAILED) {
        close(region->fd);
        region->fd = -1;
        return -1;
    }

    region->size = size;
    region->phys_addr = phys_addr;
    region->backend = SRAM_BACKEND_DEVMEM;

    return 0;
#endif
}

void sram_close(sram_region_t *region) {
    if (!region || !region->base) {
        return;
    }

    if (region->backend == SRAM_BACKEND_MOCK) {
        free((void *)region->base);
    }

#ifdef __linux__
    if (region->backend == SRAM_BACKEND_DEVMEM) {
        munmap((void *)region->base, region->size);
        if (region->fd >= 0) {
            close(region->fd);
        }
    }
#endif

    region->fd = -1;
    region->base = NULL;
    region->size = 0;
    region->phys_addr = 0;
}

uint32_t sram_read32(sram_region_t *region, size_t offset) {
    if (!region || !region->base || offset + sizeof(uint32_t) < offset || offset + sizeof(uint32_t) > region->size) {
        return 0;
    }

    volatile uint32_t *ptr = (volatile uint32_t *)(region->base + offset);
    return *ptr;
}

void sram_write32(sram_region_t *region, size_t offset, uint32_t value) {
    if (!region || !region->base || offset + sizeof(uint32_t) < offset || offset + sizeof(uint32_t) > region->size) {
        return;
    }

    volatile uint32_t *ptr = (volatile uint32_t *)(region->base + offset);
    *ptr = value;
}

int sram_copy_to(sram_region_t *region, size_t offset, const void *src, size_t len) {
    if (!region || !region->base || !src || offset + len < offset || offset + len > region->size) {
        return -1;
    }

    memcpy((void *)(region->base + offset), src, len);
    return 0;
}

int sram_copy_from(sram_region_t *region, size_t offset, void *dst, size_t len) {
    if (!region || !region->base || !dst || offset + len < offset || offset + len > region->size) {
        return -1;
    }

    memcpy(dst, (const void *)(region->base + offset), len);
    return 0;
}
