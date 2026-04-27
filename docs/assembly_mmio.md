# Assembly-backed MMIO & Memory Barriers

This document explains why explicit ordering is critical when interacting with SRAM-like residency regions and Memory Mapped I/O (MMIO).

## The Core Concept

In a memory-control-plane architecture, the important idea is not just the store itself, but the **ordering** of operations.

### The Access Pattern
To ensure a device correctly observes a sequence of operations, we use an "Ordered Access" pattern:

```text
barrier → store/load → barrier
```

## Why Volatile?

The `volatile` keyword tells the C compiler that the load or store is "device-visible" and must not be optimized away or combined with other operations. However, `volatile` alone does **not** prevent the CPU hardware from reordering operations.

## Barriers

1. **Compiler Barrier**: Prevents the compiler from moving instructions across the barrier during optimization.
2. **CPU Memory Barrier**: Prevents the hardware (out-of-order execution engine) from reordering memory operations at runtime.

### MMIO vs. Cached RAM
Ordinary RAM is cached and follows cache-coherency rules. MMIO apertures and SRAM buffers often behave differently:
- They may be uncacheable.
- They may require strictly ordered writes (e.g., writing a payload *before* writing a "ready" bit).
- They may not participate in the standard coherence protocol.

## Architecture Implementation

### x86_64: `mfence`
On x86, we use `mfence` for full memory serialization. We also provide a `pause` helper for busy-wait loops.

### aarch64: `dmb sy`
On ARM64, we use `dmb sy` (Data Memory Barrier, Full System). We also provide a `yield` helper.

## Prototype Status

This implementation is a prototype and a design sketch. Real-world production drivers may use platform-specific kernel primitives (like `rmb()`, `wmb()`, or `iowrite32()`) instead of raw inline assembly to handle the specific requirements of the bus (e.g., PCIe vs. AXI).
