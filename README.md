# RISC-V Baremetal SiFive U Demo

![Architecture](https://img.shields.io/badge/Architecture-RISC--V-orange)
![Emulation](https://img.shields.io/badge/Emulation-QEMU-blue)
![Software](https://img.shields.io/badge/Software-Baremetal-success)
![License](https://img.shields.io/badge/License-MIT-yellow)

A clean, modular, and well-documented baremetal demonstration for the SiFive FU540 (sifive_u) board. This project implements UART communication, formatted output, and a "Walkie-Talkie" mode for cross-instance communication over TCP.

## Prerequisites

To build and run this project, you need:
-   **QEMU**: Specifically `qemu-system-riscv64`.
-   **RISC-V GNU Toolchain**: A cross-compiler for `riscv64-unknown-elf`.

## Installation Guide

### Linux (Debian / Ubuntu)
```bash
sudo apt update
sudo apt install qemu-system-riscv64 gcc-riscv64-unknown-elf libnewlib-dev
```

### Linux (Fedora)
```bash
sudo dnf install qemu-system-riscv gcc-riscv64-elf
```

### Linux (Arch)
```bash
sudo pacman -S qemu-system-riscv riscv64-elf-gcc
```

### macOS (Homebrew)
```bash
brew install qemu
brew install riscv-gnu-toolchain
```
*Note: Ensure the toolchain's `bin` directory is in your `PATH`.*

---

## Building from Source

If your distribution does not provide the required packages, you can build them from the official sources:

-   **QEMU**: [QEMU Build Documentation](https://www.qemu.org/docs/master/devel/build-system.html)
-   **RISC-V Toolchain**: [RISC-V GNU Toolchain GitHub](https://github.com/riscv-collab/riscv-gnu-toolchain)

---

## Usage

The project supports two main modes: a **Normal Demo** and a **Walkie-Talkie Mode**.

### 1. Normal Demo
This mode prints motivational quotes, poetry, and numeric formatting tests (Decimal, Hex, Binary) to the console.

**Build:**
```bash
make clean && make all
```

**Run:**
```bash
make qemu
```

### 2. Walkie-Talkie Mode
This mode enables cross-instance communication using UART1 redirected over a TCP socket. Anything typed into the UART1 of one instance can be echoed to the UART0 (console) of another.

**Build:**
```bash
make clean && make WALKIE_TALKIE_MODE=1 all
```

**Run (Multi-Instance Setup):**

1.  **Terminal 1 (Server)**:
    ```bash
    make qemu-talk-server
    ```
2.  **Terminal 2 (Client)**:
    ```bash
    make qemu-talk-client
    ```

> [!TIP]
> To interact with the TCP stream directly or if you are using PTY redirection, you can use the `screen` command:
> - **TCP Connection**: `screen localhost 6969`
> - **PTY Connection**: `screen /dev/pts/X` (where X is the number reported by QEMU).
> - *To exit screen, use `Ctrl+A` followed by `K`.*

---

## Project Structure

-   `main.c`: Application entry point and demo logic.
-   `helpers.c / .h`: Formatted output and UART utility functions.
-   `uart.h`: Hardware register definitions for SiFive FU540.
-   `dstartup.s`: RISC-V assembly startup script (stack setup, hart parking).
-   `d-riscv.ld`: Linker script defining the memory layout (Entry at `0x80000000`).
-   `Makefile`: Dynamic build system with auto-discovery of source files.

---

## Hardware References

For detailed hardware specifications of the SiFive FU540-C000 (the SoC on the HiFive Unleashed board), please refer to the official datasheet:

-   [SiFive FU540-C000 Manual](https://pdos.csail.mit.edu/6.828/2025/readings/FU540-C000-v1.0.pdf)

---

## License
MIT License. See `LICENSE` for details.
