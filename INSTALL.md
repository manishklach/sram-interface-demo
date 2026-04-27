# Installation & Usage

## Build Instructions

This project is built using standard `make` and GCC. 

### Prerequisites (Linux/WSL)

Install build essentials:
```bash
sudo apt update
sudo apt install build-essential
```

### Compiling

Build the library, the main demo, and the examples:
```bash
make all
```

Other available targets:
* `make demo`: Builds just the `sram_demo` executable.
* `make examples`: Builds the binaries in `examples/`.
* `make test`: Runs the local mock-mode tests.
* `make clean`: Removes generated binaries.

---

## Running the Demo

### 1. Mock Mode (Default)
The demo defaults to mock mode, simulating an SRAM region using `calloc`. This runs safely on any system:
```bash
./sram_demo
```

### 2. Hardware Mode (MMIO /dev/mem)
To access real physical memory on an FPGA, SoC, or bare-metal system:
```bash
sudo ./sram_demo --devmem 0x40000000 0x10000
```
> **⚠️ WARNING**: Using `/dev/mem` can crash your system or corrupt hardware state. Do not use this in production.

---

## Running the Examples

After building with `make examples` or `make all`, you can run the simulated AI infrastructure examples:

```bash
./kv_cache_tile_demo
./tensor_tile_demo
```
These examples run safely in mock mode by default and demonstrate the `mem_hint` API.
