# Architecture

This document describes the memory-control-plane architecture modeled in this repository.

## Overview

Modern CPUs rely on implicit locality: applications allocate memory, and the hardware transparently moves it through L1/L2/L3 caches.

In contrast, specialized hardware (like AI accelerators, DPUs, or tightly-coupled FPGAs) often features explicitly managed scratchpad memory (SRAM). Software must decide what goes into this memory and when.

This prototype demonstrates a software stack for explicit memory residency:

1. **Application / AI Runtime**: Decides *what* is hot (e.g., a specific KV-cache tile or MoE expert).
2. **`mem_hint` API**: The user-space API for reserving and binding memory to tiers.
3. **Backend Selector**: Routes requests to either a safe Mock SRAM (for development) or a raw MMIO `/dev/mem` aperture (for physical hardware).

## Diagram

See [architecture.svg](architecture.svg) for a visual representation of this stack.
