#include "sram_mmio.h"
#include <stdio.h>
#include <string.h>
int main() {
    sram_region_t sram;
    if (sram_open(&sram, 0x40000000, 0x10000) != 0) {
        printf("Failed to map SRAM\n");
        return 1;
    }
    sram_write32(&sram, 0x0, 0xDEADBEEF);
    printf("Read: 0x%X\n", sram_read32(&sram, 0x0));
    const char *msg = "hello SRAM";
    char buf[64] = {0};
    sram_copy_to(&sram, 0x100, msg, strlen(msg)+1);
    sram_copy_from(&sram, 0x100, buf, sizeof(buf));
    printf("String: %s\n", buf);
    sram_close(&sram);
    return 0;
}
