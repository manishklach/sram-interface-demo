# Contributing to SRAM Interface Demo

We welcome contributions that improve the credibility and utility of this memory-control-plane prototype.

## Principles
1. **Plain C**: Use C99 only. Avoid complex language features.
2. **No Dependencies**: Do not add external libraries or complex build systems.
3. **Portability**: Ensure the **Mock Mode** continues to work on any machine (Linux, macOS, Windows via WSL).
4. **Quiet Code**: The low-level library (`src/mem_hint.c`, `src/sram_mmio.c`) must be quiet. Use return codes for errors.
5. **Small PRs**: Keep changes focused and easy to review.

## Workflow
- Create a feature branch.
- Ensure `make clean && make && make test` passes in a Linux environment.
- Avoid overclaiming performance; clearly label any simulated or mock-mode metrics.
- Note that raw `/dev/mem` is a prototype-only backend.

## Roadmap & Features
If you are suggesting a new backend (e.g., UIO/VFIO) or an AI runtime example, please open a feature request first to discuss the design.

Thank you for helping us explore explicit memory residency!
