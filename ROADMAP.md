# Roadmap

This document outlines the phased development of the memory-control-plane stack.

## Phase 1: Mock SRAM Backend (Complete)
* Basic API for reading, writing, and copying to a simulated SRAM region using `calloc`.
* Allows for rapid prototyping and testing of the `mem_hint` layer on standard hardware.

## Phase 2: MMIO/devmem Backend (Complete)
* Hardware integration via `/dev/mem` mapping.
* Exposes a real physical aperture (e.g., FPGA BRAM, PCIe BAR) to the userspace runtime.
* *Note: Requires root and is for prototyping only.*

## Phase 3: UIO / VFIO Backend (Planned)
* Transition from raw `/dev/mem` to safe userspace driver frameworks like UIO or VFIO.
* Allows specific physical regions to be mapped securely without exposing the entire system memory map to userspace.

## Phase 4: `/dev/mem_hint` Kernel Driver Stub (Planned)
* Design and implement a minimal character device.
* Support for basic `ioctl` commands: `MEM_HINT_RESERVE`, `MEM_HINT_BIND`.
* See [dev_mem_hint.md](docs/dev_mem_hint.md) for details.

## Phase 5: Compiler / Runtime Integration (Planned)
* Develop a basic runtime that uses the `mem_hint` API to manage a memory pool.
* Connect the runtime to a simple tensor compiler to demonstrate automated tile staging.

## Phase 6: AI Runtime Examples (Planned)
* Implement realistic simulations of KV cache management.
* Implement expert weight routing scenarios using the explicit residency API.
