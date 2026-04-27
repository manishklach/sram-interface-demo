# SRAM Interface Demo v2

A small GitHub-ready prototype showing two layers:

1. **Raw SRAM interface** using MMIO-style read/write APIs.
2. **Memory hint / residency API** showing how software could request SRAM placement for important data.

The demo now includes a **mock SRAM backend**, so it can run on Windows, macOS, Linux, WSL, and CI without real hardware.

---

## Why This Exists

Modern CPUs hide memory placement behind caches and coherency machinery.

A memory-centric runtime may want something more explicit:

```text
Runtime/compiler says:
"this KV-cache tile is hot; keep it in SRAM"

Memory control plane says:
"bind this object to an SRAM-backed region"

Hardware/software substrate says:
"write it into the SRAM aperture"
```

This repo is a tiny prototype of that stack.

---

## Project Structure

```text
sram-interface-demo/
├── include/
│   ├── sram_mmio.h
│   └── mem_hint.h
├── src/
│   ├── sram_mmio.c
│   ├── mem_hint.c
│   └── demo.c
├── Makefile
└── README.md
```

---

## Build on Windows PowerShell

Install GCC using MSYS2 or MinGW.

A simple option:

```powershell
winget install msys2
```

Then open an MSYS2 MinGW terminal, or use any terminal where `gcc` is available.

From the repo folder:

```powershell
gcc -O2 -Wall -Wextra -Iinclude src\demo.c src\sram_mmio.c src\mem_hint.c -o sram_demo.exe
```

Run:

```powershell
.\sram_demo.exe
```

Expected output:

```text
Opening mock SRAM backend size=0x10000

=== Raw SRAM MMIO-style API ===
SRAM[0x00] = 0xDEADBEEF
SRAM[0x04] = 0xCAFEBABE

=== Memory Hint / Residency API Demo ===
[mem_hint] reserve name=kv_cache_tile_L8_H3 tier=SRAM size=37 offset=0x100
[mem_hint] bind kv_cache_tile_L8_H3 to SRAM at offset=0x100 size=37
[mem_hint] read kv_cache_tile_L8_H3 from SRAM at offset=0x100 size=37
Readback payload: KV_CACHE_TILE: token=42 layer=8 head=3
```

---

## Build on Linux / WSL

```bash
sudo apt update
sudo apt install build-essential
make
./sram_demo
```

The default mode uses mock SRAM.

---

## Run Against Real Hardware SRAM

This requires Linux and an MMIO-exposed SRAM region.

```bash
sudo ./sram_demo --devmem 0x40000000 0x10000
```

Arguments:

```text
./sram_demo --devmem <physical_sram_base> <size>
```

Example:

```bash
sudo ./sram_demo --devmem 0x40000000 0x10000
```

---

## Important Safety Warning

The `--devmem` mode uses `/dev/mem`.

That can be dangerous.

It can:

- crash the machine
- corrupt hardware state
- violate platform security assumptions
- fail on normal laptops/desktops

For production systems, use:

- UIO
- VFIO
- PCIe BAR mapping
- a custom kernel driver
- a proper `/dev/mem_hint` interface

---

## API Example

Raw SRAM-style API:

```c
sram_write32(&sram, 0x00, 0xDEADBEEF);
uint32_t value = sram_read32(&sram, 0x00);
```

Memory hint API:

```c
mem_hint_region_t hint;

mem_hint_reserve(&hint,
                 "kv_cache_tile_L8_H3",
                 MEM_TIER_SRAM,
                 sizeof(payload),
                 0x100);

mem_hint_bind_to_sram(&sram, &hint, payload);
mem_hint_read_from_sram(&sram, &hint, readback);
```

---

## Conceptual Stack

```text
Application / AI Runtime
        ↓
Memory Hint API
        ↓
SRAM Residency Binding
        ↓
MMIO / UIO / VFIO / Kernel Driver
        ↓
Physical SRAM
```

---

## Why Mock SRAM Matters

Mock SRAM lets this repo run anywhere.

That makes it useful for:

- GitHub demos
- CI tests
- API design
- explaining the concept
- showing the future `/dev/mem_hint` direction without needing FPGA hardware

---

## Future Direction

This can evolve into a real memory-control-plane prototype:

```c
mem_hint_reserve("kv_cache", SRAM_TIER);
mem_hint_bind(ptr, SRAM_REGION_2);
mem_hint_promote(ptr, SRAM_TIER);
mem_hint_evict(ptr, DRAM_TIER);
```

Possible next steps:

- add a Linux kernel driver stub
- expose `/dev/mem_hint`
- add `ioctl()` commands for reserve/bind/promote/evict
- add a Python wrapper
- add a small AI-runtime example with KV-cache tiles
- add GitHub Actions for mock-mode build testing

---

## License

MIT
