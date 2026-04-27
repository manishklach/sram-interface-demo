# SRAM Interface Demo: A Memory-Control-Plane Prototype

A tiny systems prototype showing how software can explicitly bind hot data to SRAM-like residency regions using mock and MMIO backends.

## What this is
- A small prototype of a memory control plane.
- A demonstration of explicit data placement into SRAM-like regions.
- A dual-backend library supporting both a safe Mock SRAM and an optional MMIO `/dev/mem` backend.

## What this is NOT
- A production-ready memory driver.
- A kernel-level residency manager (see [docs/dev_mem_hint.md](docs/dev_mem_hint.md)).
- A replacement for standard CPU caching or coherence protocols.

## Why this exists
Standard CPUs hide memory placement behind multiple layers of abstraction (L1/L2/L3 caches, reorder buffers, and hardware prefetchers). While efficient for general-purpose code, AI-centric workloads—such as KV caches, MoE expert routing, and tensor tiling—often require explicit control over residency to achieve bounded latency and predictable performance.

This repository demonstrates how software can bypass implicit hardware heuristics to manage memory residency explicitly.

## Quickstart

Build and run the demo on any Linux or WSL environment:

```bash
make
./sram_demo
```

Example output:
```text
[mem_hint] reserve "kv_tile_0"
[mem_hint] bind → SRAM offset 0x100
[mem_hint] write → 39 bytes
[mem_hint] readback → verified ✔
```

## Backends

| Backend | Implementation | Use Case |
| :--- | :--- | :--- |
| **Mock** | Userspace `calloc` | Local development, CI, and architecture testing. |
| **MMIO** | `/dev/mem` mapping | Real hardware (FPGA BRAM, PCIe BAR, SoC SRAM). |

## Architecture
See [docs/architecture.md](docs/architecture.md) for the high-level system design and stack overview.

## Use Cases
Detailed scenarios for KV caches, MoE experts, and tensor staging are available in [docs/use_cases.md](docs/use_cases.md).

## Safety Notes
- `/dev/mem` access requires root and can crash your system.
- This project is for educational and prototyping purposes only.
- Always use the Mock backend for development.

## Roadmap
- **Phase 1**: Mock SRAM backend
- **Phase 2**: /dev/mem backend
- **Phase 3**: UIO/VFIO secure mapping
- **Phase 4**: `/dev/mem_hint` kernel driver stub
- **Phase 5**: Compiler/runtime integration
- **Phase 6**: AI runtime reference examples

## License
[MIT](LICENSE)
