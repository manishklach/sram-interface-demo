# Use Cases for Explicit Residency

This memory-control-plane prototype is designed for specific hardware/software co-design scenarios where standard caching is insufficient or suboptimal.

## KV-Cache Residency (LLM Inference)
In Large Language Models, the KV cache grows dynamically and is repeatedly accessed during token generation. Explicitly binding the active blocks of the KV cache to fast SRAM (or HBM) rather than standard DRAM prevents cache thrashing and guarantees bounded latency for critical attention layers.

## MoE Expert Weight Promotion
Mixture of Experts (MoE) models route tokens to specific expert networks. If an expert is frequently activated, the runtime can proactively promote its weights into SRAM before the next forward pass. If the load shifts, it can evict it to DRAM or CXL.

## Tensor Tile Staging
During large matrix multiplications (e.g., in convolutions or dense attention), data must be tiled to fit into on-chip memory. An explicit residency API allows a compiler to stage tiles perfectly, overlapping computation with data movement without relying on heuristic-based hardware prefetchers.

## FPGA Scratchpad Memory
FPGAs often expose internal BRAM or UltraRAM as memory-mapped I/O. Since these regions do not participate in CPU cache coherency, software must use explicit interfaces to stream data in and out.

## Near-Memory Accelerator Control
Smart memory devices (like CXL-attached accelerators or processing-in-memory chips) have their own local fast memory. The host CPU can use this control plane to push operands into the accelerator's local SRAM, trigger computation, and pull the results back.

## Compiler-Directed Memory Placement
Modern AI compilers (like XLA or Triton) have full visibility into the data flow graph. Instead of emitting standard memory allocation calls, the compiler can emit memory intents, reserving SRAM precisely when a tensor is generated and evicting it immediately after its last use.
