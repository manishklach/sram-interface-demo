# Cache and TLB Notes for Explicit Residency

This document explains the interaction between explicit memory residency (SRAM/MMIO) and the CPU's implicit memory hierarchy (Caches and TLBs).

## Ordered MMIO Access

Unlike standard DRAM, MMIO regions (including SRAM apertures) are often mapped as **uncacheable** or **write-combining**. This means the hardware does not guarantee that operations will be observed in the order they appear in the C code.
- **Full Barriers**: Instructions like `mfence` (x86) or `dmb sy` (ARM) are used to ensure all previous memory operations are visible before continuing.
- **Compiler Barriers**: These prevent the compiler from reordering instructions but do not affect the CPU hardware.

## Cache Line Maintenance

Modern CPUs provide hints and instructions for managing the cache hierarchy from user-space:
- **`clflush` (x86)**: Flushes a specific cache line from all levels of the cache hierarchy to main memory. Useful when preparing a buffer that will be accessed by a non-coherent device (like an FPGA).
- **Prefetching**: Instructions like `prefetcht0` (x86) or `prfm` (ARM) suggest to the CPU that specific data will be needed soon, potentially reducing latency by fetching it into the cache before it is explicitly requested.

## TLB and Virtual Memory

The Translation Lookaside Buffer (TLB) caches the mapping between virtual addresses and physical addresses.
- **User-space Visibility**: Applications only see virtual addresses. The physical address where an SRAM block is mapped is hidden by the OS page tables.
- **Privileged Operations**: Modifying page tables or invalidating the TLB requires supervisor (kernel) privileges. 
- **The Role of `/dev/mem_hint`**: In a production system, a dedicated kernel driver would manage the physical SRAM pool and update page tables securely, allowing a user-space AI runtime to "bind" virtual memory to specific physical SRAM residency without needing raw `/dev/mem` access.

## Summary: What User-space Can Do

| Feature | User-space Support | Mechanism |
| :--- | :--- | :--- |
| **Ordering** | Full | `mfence`, `dmb` |
| **Cache Hints** | Hints only | `clflush`, `prefetch` |
| **Timing** | Full | `rdtsc`, `cntvct` |
| **Page Tables** | None | Requires Kernel/Driver |
| **TLB Control** | None | Requires Kernel/Driver |

*Note: This repository is a prototype. In a real system, these low-level operations are often abstracted by a hardware abstraction layer (HAL) or a kernel driver.*
