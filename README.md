# OS-final-project

xv6 Kernel Enhancements with New System Calls : Add system calls to provide useful system information.

## Overview
This repository contains modifications to the xv6 operating system kernel that introduce new system calls for exposing useful runtime and system-level information.

## Goals
- Extend xv6 with additional system calls.
- Provide system information to user programs through well-defined kernel interfaces.
- Practice kernel-level development concepts such as syscall wiring, kernel/user boundary handling, and process-related introspection.

## Features
Potential enhancements in this project include:
- New kernel system calls to query system or process state.
- User-space test programs to validate syscall behavior.
- Kernel changes across syscall table, handlers, and headers.

## Repository Structure (typical xv6 layout)
- `kernel/` – Core kernel source code, syscall implementations, process/memory management.
- `user/` – User-space programs and syscall usage tests.
- `Makefile` – Build and run targets.

## Build and Run
1. Ensure required dependencies are installed (e.g., `gcc`, `make`, `qemu`).
2. Build xv6:
   ```bash
   make
   ```
3. Run xv6 in QEMU:
   ```bash
   make qemu
   ```

## Testing
After booting xv6, run the user programs that exercise the new system calls and verify expected output.

## Example Workflow for Adding a System Call
1. Define syscall number in kernel syscall definitions.
2. Add userspace stub/prototype.
3. Implement kernel handler.
4. Register handler in syscall dispatch table.
5. Add and run tests from user space.

## Learning Outcomes
- Understanding of OS syscall mechanisms.
- Experience modifying a small Unix-like kernel.
- Exposure to kernel debugging and validation workflows.

## Contributing
Contributions are welcome. Please open an issue or submit a pull request with a clear description of your changes.

## License
If this project is part of coursework, follow your course guidelines for sharing and reuse.
