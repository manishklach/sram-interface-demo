name: Bug Report
description: Report a bug in the SRAM interface demo or mem-hint prototype.
body:
  - type: textarea
    id: description
    attributes:
      label: Bug Description
      description: A clear and concise description of what the bug is.
    validations:
      required: true
  - type: dropdown
    id: mode
    attributes:
      label: Operating Mode
      options:
        - Mock SRAM (default)
        - /dev/mem MMIO (hardware)
    validations:
      required: true
  - type: input
    id: environment
    attributes:
      label: Environment
      placeholder: e.g., Ubuntu 22.04, WSL2, x86_64, aarch64
  - type: textarea
    id: steps
    attributes:
      label: Steps to Reproduce
      description: Exactly what commands did you run?
  - type: textarea
    id: logs
    attributes:
      label: Output/Logs
      description: Copy-paste the terminal output here.
