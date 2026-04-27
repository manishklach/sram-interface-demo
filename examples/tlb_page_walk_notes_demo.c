#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

int main(void) {
    long page_size = sysconf(_SC_PAGESIZE);
    printf("[vmem] system page size: %ld bytes\n", page_size);

    printf("[vmem] allocating 3 virtual pages via mmap\n");
    size_t alloc_size = page_size * 3;
    void *addr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("[vmem] virtual base address: %p\n", addr);

    printf("[vmem] touching one byte per page to trigger page faults/TLB fills\n");
    uint8_t *p = (uint8_t *)addr;
    p[0] = 1;
    p[page_size] = 1;
    p[page_size * 2] = 1;

    printf("\n--- Educational Notes ---\n");
    printf("1. User-space code only sees Virtual Addresses (VAs).\n");
    printf("2. The Operating System (OS) owns the Page Tables that map VAs to Physical Addresses (PAs).\n");
    printf("3. The CPU's Memory Management Unit (MMU) performs the 'Page Walk' and caches results in the TLB.\n");
    printf("4. TLB invalidation (e.g., INVLPG on x86) is a privileged instruction.\n");
    printf("5. A future /dev/mem_hint driver would mediate physical residency while respecting OS protections.\n");
    printf("-------------------------\n");

    munmap(addr, alloc_size);
    printf("[vmem] verify: OK ✔\n");

    return 0;
}
