# INSTALL

## Quick Start: Windows PowerShell

Make sure `gcc` is installed and available:

```powershell
gcc --version
```

Build:

```powershell
gcc -O2 -Wall -Wextra -Iinclude src\demo.c src\sram_mmio.c src\mem_hint.c -o sram_demo.exe
```

Run:

```powershell
.\sram_demo.exe
```

This uses the mock SRAM backend.

---

## Quick Start: Linux / WSL

```bash
sudo apt update
sudo apt install build-essential
make
./sram_demo
```

---

## Real Hardware Mode

Only use this on Linux systems with an SRAM region exposed through physical memory.

```bash
sudo ./sram_demo --devmem 0x40000000 0x10000
```
