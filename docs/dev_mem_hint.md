# The Future: `/dev/mem_hint`

This repository currently uses `/dev/mem` for hardware-level physical address mapping. **This is unsafe and unsuitable for production environments.**

To provide a safe, user-space interface to explicit memory residency, the roadmap for this memory-control-plane architecture includes a dedicated Linux character device: `/dev/mem_hint`.

*(Note: This driver is conceptual and under design. It is not implemented in this demo.)*

## Why `/dev/mem_hint`?

Raw `/dev/mem` allows a user process to read or write any physical memory, bypassing OS security, page tables, and protections. It can crash the kernel or corrupt other processes.

`/dev/mem_hint` would act as a safe proxy. The kernel driver would:
1. Manage a pool of hardware SRAM or CXL regions.
2. Ensure processes can only map regions they have successfully reserved.
3. Track ownership and orchestrate evictions if memory runs out.

## Proposed `ioctl` Interface

The character device would expose an API via `ioctl` commands:

* `MEM_HINT_RESERVE`: Request a block of memory in a specific tier (SRAM, CXL).
* `MEM_HINT_BIND`: Bind an allocated virtual memory range to the reserved tier.
* `MEM_HINT_PROMOTE`: Instruct the driver to actively copy memory into the fast tier.
* `MEM_HINT_EVICT`: Instruct the driver to copy memory back to DRAM and free the tier.
* `MEM_HINT_QUERY`: Check if a specific region is currently resident in the fast tier.

## Safety and Orchestration

By moving the control plane to a dedicated driver, the kernel retains ultimate authority while allowing the AI runtime or compiler to explicitly dictate memory intent. This enables true hardware/software codesign for memory management without sacrificing system stability.
