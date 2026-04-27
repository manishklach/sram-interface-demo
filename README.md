# SRAM Interface Demo

**A tiny memory-control-plane prototype showing how software can explicitly bind hot data to SRAM-like residency regions.**

## 🧠 Concept

SRAM is not accessed like normal memory in userspace. It must be exposed through hardware interfaces such as MMIO regions, PCIe BARs, or `/dev/mem` (used here for prototyping).

This project demonstrates:
* Mapping a physical SRAM region into userspace
* Performing explicit reads/writes and buffer copies
* Building a foundation for **memory-centric runtimes and explicit residency control**

## 🆚 What this is / What this is not

**What this is:**
* An educational design sketch and prototype for hardware/software codesign.
* A demonstration of how AI runtimes and compilers might explicitly route memory.
* A low-level mock and MMIO interface layer.

**What this is not:**
* A production-ready memory driver.
* A replacement for CPUs or standard caching.
* Safe to use on production systems (`/dev/mem` can crash your hardware).

## 🕵️‍♂️ Why CPUs hide this

CPUs normally rely on caches, reorder buffers, coherence, and implicit locality. You access an address, and the hardware transparently decides where it lives. 

This repository demonstrates **explicit placement** instead. Instead of relying on hidden cache behavior, software can explicitly control:
* What data is resident
* Where it lives (SRAM vs DRAM vs CXL)
* When it is promoted or evicted
* Which compute unit owns it

## 📁 Project Structure

```
sram-interface-demo/
├── docs/                 # Architecture diagrams, use cases, and design docs
├── examples/             # AI runtime examples (KV cache, tensor staging)
├── include/              # Header files (sram_mmio, mem_hint)
├── src/                  # Core library and demo implementation
├── Makefile              # Build system
├── INSTALL.md            # Installation and build instructions
└── README.md             # This file
```

## ⚙️ Getting Started

See [INSTALL.md](INSTALL.md) for build instructions and how to run the demo.

## 🚀 Future Extensions

This repo represents the lowest layer of a memory-control-plane architecture. Future phases (detailed in [ROADMAP.md](ROADMAP.md)) will explore:
* `/dev/mem_hint` kernel character device
* Compiler-driven placement (Memory Intent IR)
* Deeper AI runtime integration

## 🧾 License

[MIT License](LICENSE)
