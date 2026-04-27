# Assembly-backed MMIO & Memory Barriers

This document explains the architecture-specific layer used in this prototype for ordered 32-bit SRAM/MMIO access.

## Why `volatile`?

In C, the `volatile` keyword tells the compiler that the value of a variable may change at any time—without any action being taken by the code the compiler finds nearby. For MMIO (Memory Mapped I/O), this is critical because:
1. **Device Visibility**: A write to an MMIO address must actually happen on the hardware bus; the compiler cannot "optimize away" the write even if the value is never read back in the software.
2. **Side Effects**: Reading from an MMIO address might trigger a hardware action (like clearing an interrupt or popping a FIFO).

## Memory Barriers

Even with `volatile`, modern CPUs can reorder memory operations for performance. For example, a CPU might try to read data from SRAM before the command to "prepare" that data has actually reached the device.

### Compiler Barriers vs. CPU Barriers

- **Compiler Barrier**: Tells the compiler not to reorder instructions across the barrier during optimization. It does *not* prevent the CPU hardware from reordering at runtime.
- **CPU Memory Barrier**: An actual hardware instruction that ensures all memory operations before the barrier are completed before any operations after the barrier begin.

## Architecture Implementation

This prototype implements `sram_arch_memory_barrier()` using inline assembly for major architectures:

### x86_64: `mfence`
On x86, the `mfence` instruction is used to serialize all load and store operations that were issued prior to the instruction.

### aarch64 (ARMv8): `dmb sy`
On ARM, the `dmb sy` (Data Memory Barrier, Full System) instruction ensures that all explicit memory accesses before the barrier are observed by all observers in the system before any explicit memory accesses after the barrier.

### Portable Fallback
On unknown architectures, we fall back to `__sync_synchronize()`, which is a GCC builtin that issues a full memory barrier.

## Limitations

This implementation is a **prototype** and a **design sketch**. Real hardware integration often requires stronger, platform-specific ordering rules (e.g., PCIe ordering, non-posted vs. posted writes). For production systems, a real kernel driver or a specialized userspace framework (like UIO/VFIO) is required to handle these complexities safely.
