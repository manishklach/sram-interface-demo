name: Feature Request
description: Suggest a new feature or backend for the memory-control-plane.
body:
  - type: textarea
    id: use-case
    attributes:
      label: Use Case
      description: What specific hardware or AI scenario is this for?
    validations:
      required: true
  - type: textarea
    id: proposal
    attributes:
      label: Proposed API / Change
      description: How should the mem_hint API or backend layer change?
  - type: textarea
    id: why
    attributes:
      label: Why it belongs in this prototype
      description: How does this align with the "explicit residency" goal?
