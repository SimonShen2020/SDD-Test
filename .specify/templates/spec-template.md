# Feature Specification: [FEATURE NAME]

**Feature Branch**: `[###-feature-name]`  
**Created**: [DATE]  
**Status**: Draft  
**Input**: User description: "$ARGUMENTS"

## User Scenarios & Testing *(mandatory)*

<!--
  IMPORTANT: User stories should be PRIORITIZED as user journeys ordered by importance.
  Each user story/journey must be INDEPENDENTLY TESTABLE - meaning if you implement just ONE of them,
  you should still have a viable MVP (Minimum Viable Product) that delivers value.
  
  Assign priorities (P1, P2, P3, etc.) to each story, where P1 is the most critical.
  Think of each story as a standalone slice of functionality that can be:
  - Developed independently
  - Tested independently
  - Deployed independently
  - Demonstrated to users independently
-->

### User Story 1 - [Brief Title] (Priority: P1)

[Describe this user journey in plain language]

**Why this priority**: [Explain the value and why it has this priority level]

**Independent Test**: [Describe how this can be tested independently - e.g., "Can be fully tested by [specific action] and delivers [specific value]"]

**Acceptance Scenarios**:

1. **Given** [initial state], **When** [action], **Then** [expected outcome]
2. **Given** [initial state], **When** [action], **Then** [expected outcome]

---

### User Story 2 - [Brief Title] (Priority: P2)

[Describe this user journey in plain language]

**Why this priority**: [Explain the value and why it has this priority level]

**Independent Test**: [Describe how this can be tested independently]

**Acceptance Scenarios**:

1. **Given** [initial state], **When** [action], **Then** [expected outcome]

---

### User Story 3 - [Brief Title] (Priority: P3)

[Describe this user journey in plain language]

**Why this priority**: [Explain the value and why it has this priority level]

**Independent Test**: [Describe how this can be tested independently]

**Acceptance Scenarios**:

1. **Given** [initial state], **When** [action], **Then** [expected outcome]

---

[Add more user stories as needed, each with an assigned priority]

### Edge Cases

<!--
  ACTION REQUIRED: The content in this section represents placeholders.
  Fill them out with the right edge cases.
-->

- What happens when [boundary condition]?
- How does system handle [error scenario]?

## Requirements *(mandatory)*

<!--
  ACTION REQUIRED: The content in this section represents placeholders.
  Fill them out with the right functional requirements.
-->

### Functional Requirements

- **FR-001**: System MUST [specific capability, e.g., "allow users to create accounts"]
- **FR-002**: System MUST [specific capability, e.g., "validate email addresses"]  
- **FR-003**: Users MUST be able to [key interaction, e.g., "reset their password"]
- **FR-004**: System MUST [data requirement, e.g., "persist user preferences"]
- **FR-005**: System MUST [behavior, e.g., "log all security events"]

*Example of marking unclear requirements:*

- **FR-006**: System MUST authenticate users via [NEEDS CLARIFICATION: auth method not specified - email/password, SSO, OAuth?]
- **FR-007**: System MUST retain user data for [NEEDS CLARIFICATION: retention period not specified]

### Key Entities *(include if feature involves data)*

- **[Entity 1]**: [What it represents, key attributes without implementation]
- **[Entity 2]**: [What it represents, relationships to other entities]

## Success Criteria *(mandatory)*

<!--
  ACTION REQUIRED: Define measurable success criteria.
  These must be technology-agnostic and measurable.
-->

### Measurable Outcomes

- **SC-001**: [Measurable metric, e.g., "Users can complete account creation in under 2 minutes"]
- **SC-002**: [Measurable metric, e.g., "System handles 1000 concurrent users without degradation"]
- **SC-003**: [User satisfaction metric, e.g., "90% of users successfully complete primary task on first attempt"]
- **SC-004**: [Business metric, e.g., "Reduce support tickets related to [X] by 50%"]

## C++ Specific Requirements *(optional - include for C++ projects)*

<!--
  ACTION REQUIRED: Fill out these sections for C++ projects.
  Remove this entire section if not applicable.
-->

### Memory Constraints

- **MC-001**: Maximum heap usage: [e.g., <100MB for embedded, <1GB for desktop]
- **MC-002**: Maximum stack usage per thread: [e.g., <1MB]
- **MC-003**: Memory leak tolerance: [e.g., zero tolerance for long-running processes]
- **MC-004**: RAII enforcement: All resource management must use RAII pattern

### Performance Requirements

- **PR-001**: Latency requirement: [e.g., <10ms p99 for API calls]
- **PR-002**: Throughput requirement: [e.g., >10k requests/second]
- **PR-003**: Startup time: [e.g., <500ms cold start]
- **PR-004**: Memory allocation pattern: [e.g., no allocations in hot path]

### Platform Requirements

- **PL-001**: Target operating systems: [e.g., Windows 10+, Linux (Ubuntu 20.04+), macOS 12+]
- **PL-002**: Compiler requirements: [e.g., MSVC 2022, GCC 11+, Clang 14+]
- **PL-003**: C++ standard: [e.g., C++17 preferred]
- **PL-004**: Architecture support: [e.g., x64, ARM64]

### ABI Compatibility Requirements

- **ABI-001**: ABI stability: [e.g., stable ABI for plugin interface]
- **ABI-002**: Binary compatibility: [e.g., maintain compatibility with v1.x plugins]
- **ABI-003**: Symbol visibility: [e.g., only export public API symbols]

### Build System Requirements

- **BS-001**: Build system: CMake 3.16+
- **BS-002**: Dependency management: [e.g., vcpkg, Conan, or FetchContent]
- **BS-003**: Code generation: [e.g., Qt MOC, protobuf, gRPC]
- **BS-004**: CI/CD requirements: [e.g., static analysis, sanitizers in CI]
