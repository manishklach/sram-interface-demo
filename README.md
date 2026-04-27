# SRAM Interface Demo

A minimal hardware-facing example showing how userspace software can interface with an SRAM region exposed via memory-mapped I/O (MMIO).

This project demonstrates:

* Mapping a physical SRAM region into userspace
* Performing reads/writes
* Copying buffers to/from SRAM
* Building a foundation for **memory-centric runtimes and explicit residency control**

---

## 🧠 Concept

SRAM is not accessed like normal memory in userspace. It must be exposed through hardware interfaces such as:

* MMIO region (SoC / FPGA)
* PCIe BAR (accelerators)
* AXI memory map
* UIO / VFIO driver
* Custom kernel driver
* `/dev/mem` (used here for prototyping)

This demo uses `/dev/mem` to directly map a physical address into userspace.

---

## 📁 Project Structure

```
sram-interface-demo/
├── include/
│   └── sram_mmio.h
├── src/
│   ├── sram_mmio.c
│   └── demo.c
├── Makefile
└── README.md
```

---

## ⚙️ Build & Run Instructions

### ✅ Option 1: Linux / WSL (Recommended)

This is the only way to run the code meaningfully.

#### 1. Install WSL (Windows only, one-time)

Open PowerShell as Administrator:

```powershell
wsl --install
```

Restart your machine if prompted.

---

#### 2. Open WSL

```powershell
wsl
```

---

#### 3. Install build tools

```bash
sudo apt update
sudo apt install build-essential
```

---

#### 4. Navigate to your project

```bash
cd /mnt/c/Users/<your-username>/path/to/sram-interface-demo
```

---

#### 5. Build

```bash
make
```

OR manually:

```bash
gcc -O2 -Wall -Iinclude src/demo.c src/sram_mmio.c -o sram_demo
```

---

#### 6. Run (requires root)

```bash
sudo ./sram_demo
```

---

### ⚠️ Important Notes (WSL/Linux)

* `/dev/mem` requires **root access**
* On most systems, `/dev/mem` is restricted or virtualized
* This will only fully work if:

  * You have real hardware exposing SRAM at a physical address
  * Or you are on an embedded/FPGA platform

Otherwise, expect mapping failures (this is normal)

---

## 💻 Option 2: Windows (PowerShell, Compile Only)

You can compile on Windows, but **it will not run correctly** because `/dev/mem` does not exist.

### Install GCC (MinGW)

```powershell
winget install mingw
```

### Compile

```powershell
gcc src\demo.c src\sram_mmio.c -Iinclude -o sram_demo.exe
```

### ⚠️ Runtime Limitation

Running `sram_demo.exe` will fail or crash. This mode is only useful for:

* Syntax checking
* CI builds
* Demonstrating cross-platform structure

---

## 🧪 Example Output (on real hardware)

```
Mapping SRAM region at physical address 0x40000000, size 0x10000 bytes
Writing test values to SRAM...
SRAM[0x00] = 0xDEADBEEF
SRAM[0x04] = 0xCAFEBABE
SRAM[0x08] = 0x12345678
SRAM string readback: hello from userspace to local SRAM
```

---

## ⚠️ Safety Warning

Direct `/dev/mem` access:

* Can crash your system
* Can corrupt hardware state
* Should **never be used in production**

Use instead:

* UIO driver
* VFIO
* PCIe BAR mapping
* Custom kernel driver

---

## 🚀 Why This Matters

This demo is the **lowest layer of a memory-control-plane architecture**:

```
Memory Intent (compiler/runtime)
        ↓
Residency API (future)
        ↓
SRAM MMIO Interface  ← (this repo)
        ↓
Physical SRAM
```

Instead of relying on hidden cache behavior, software can explicitly control:

* What data is resident
* Where it lives (SRAM vs DRAM vs CXL)
* When it is promoted or evicted
* Which compute unit owns it

---

## 🔥 Future Extensions

This repo can be extended into:

* `/dev/mem_hint` kernel interface
* Userspace residency APIs:

  ```c
  mem_hint_reserve("kv_cache", SRAM_TIER);
  mem_hint_bind(ptr, REGION_2);
  ```
* Compiler-driven placement (Memory Intent IR)
* AI runtime integration (KV cache / expert routing)

---

## 📌 TL;DR

* This is a **hardware-facing SRAM interface demo**
* Works fully only on Linux + real hardware
* Windows build = compile only
* Foundation for **explicit memory control systems**

---

## 🧾 License

MIT (or your preferred license)
