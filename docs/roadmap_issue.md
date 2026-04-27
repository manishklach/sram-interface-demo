# Roadmap: memory-control-plane prototype

This issue tracks the long-term design goals and implementation milestones for the `mem-hint` architecture.

### Backend Evolution
- [ ] **UIO/VFIO backend**: Transition from raw `/dev/mem` to safe userspace driver frameworks.
- [ ] **/dev/mem_hint character device sketch**: Initial kernel module to mediate residency.

### API & Interface
- [ ] **ioctl header stabilization**: Finalize `include/mem_hint_ioctl.h` for reserve/bind/promote/evict/query.
- [ ] **AArch64 validation**: Rigorous testing of memory barriers on diverse ARM hardware.

### Examples & Benchmarks
- [ ] **Larger KV-cache tile example**: Simulate a multi-block attention sequence.
- [ ] **Mock-mode benchmark improvements**: Add multi-threaded overhead analysis.
- [ ] **AI runtime integration**: Demonstrate a stubbed XLA/Triton backend using `mem_hint`.

### Documentation
- [ ] **GitHub Pages diagram updates**: Keep visuals in sync with the architecture.
- [ ] **Use case case-studies**: Detailed write-ups for MoE and Tensor tiling.
