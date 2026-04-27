# SRAM Interface Demo (Memory Control Plane Prototype)

Explicit memory residency control for hardware-facing software, demonstrating how applications can manage hot data without relying on implicit hardware caching.

## What this is
- A minimal prototype of a memory control plane.
- A demonstration of explicit data placement into SRAM-like regions.
- A dual-backend library supporting both a safe Mock SRAM and an optional MMIO `/dev/mem` backend.

## What this is NOT
- A production-ready memory driver.
- A kernel-level residency manager (see [docs/dev_mem_hint.md](docs/dev_mem_hint.md) for future directions).
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
[mem_hint] write → 42 bytes
[mem_hint] readback → verified ✔
```

## Backends

| Backend | Implementation | Use Case |
| :--- | :--- | :--- |
| **Mock** | Userspace `calloc` | Local development, CI, and architecture testing. |
| **MMIO** | `/dev/mem` mapping | Real hardware (FPGA BRAM, PCIe BAR, SoC SRAM). |

## Architecture
See [docs/architecture.md](docs/architecture.md) for the high-level system design and stack overview.

## Roadmap
- [x] Mock SRAM backend
- [x] MMIO/devmem backend
- [ ] UIO/VFIO secure mapping
- [ ] `/dev/mem_hint` kernel driver stub
- [ ] Compiler-driven placement integration
- [ ] AI runtime (KV cache/MoE) reference examples

## License
[MIT](LICENSE)
