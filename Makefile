# MIT License
# 
# Copyright (c) 2026 Darshan
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# ------------------------------------------------------------------------------
# RISC-V Baremetal Makefile for SiFive U Board (QEMU)
# ------------------------------------------------------------------------------

# Toolchain Configuration
PROG_NAME 	= riscv-dtest
RISCV64_GCC = riscv64-unknown-elf-gcc

# --- Build Modes ---
# Set WALKIE_TALKIE_MODE=1 to enable the UART walkie-talkie demo.
# Example: make WALKIE_TALKIE_MODE=1 all
ifeq ($(WALKIE_TALKIE_MODE), 1)
	MODE_FLAGS = -DWALKIE_TALKIE_MODE
endif

# --- Dynamic Discovery ---
# Recursively find all C source files in the project
# Exclude current and hidden directories from the search if necessary
SRCS := $(shell find . -name "*.c")

# Generate the list of object files from the source files
OBJS := $(SRCS:.c=.o) dstartup.o

# Recursively find all directories to add as include paths
# This ensures that headers in sub-directories are always found.
INC_DIRS := $(shell find . -type d)
INC_FLAGS := $(foreach dir, $(INC_DIRS), -I$(dir))

# --- Compilation Flags ---
# -march=rv64g: Generate code for 64-bit RISC-V with 'G' (IMAFD) extensions.
# -mabi=lp64: Use the 64-bit LP ABI.
# -static: Link as a static binary.
# -mcmodel=medany: Medium-any code model (allows code/data to be anywhere in a 2GB range).
# -fvisibility=hidden: Reduce binary size by hiding non-global symbols.
# -fno-builtin: Disable compiler built-in functions.
# -nostdlib -nostartfiles: Bypass standard C libraries and default startup.
# -g: Include debugging information.
GCC_FLAGS	= -march=rv64g -mabi=lp64 -static -mcmodel=medany \
			  -fvisibility=hidden -fno-builtin \
			  -nostdlib -nostartfiles -g $(INC_FLAGS) $(MODE_FLAGS)

# --- Linker Flags ---
# -T d-riscv.ld: Use the custom linker script.
LD_FLAGS = -T d-riscv.ld

.PHONY: help all clean qemu debug compile qemu-talk-server qemu-talk-client

# --- Build Rules ---

all: $(PROG_NAME) ## Build the project (default)

compile: $(PROG_NAME) ## Alias for building project files

# Generic pattern rule for C source files
%.o: %.c
	@echo "CC $<"
	@$(RISCV64_GCC) $(GCC_FLAGS) -c $< -o $@

# Specific rule for the assembly startup script
dstartup.o: dstartup.s
	@echo "AS $<"
	@$(RISCV64_GCC) $(GCC_FLAGS) -c dstartup.s -o dstartup.o

# Linking step: Depends on all discovered object files
$(PROG_NAME): $(OBJS)
	@echo "LD $@"
	@$(RISCV64_GCC) $(GCC_FLAGS) -o $@ $(OBJS) $(LD_FLAGS)

clean: ## Remove all object files and the final binary
	@echo "Cleaning build artifacts..."
	@rm -f $(OBJS) $(PROG_NAME)

# --- QEMU Execution Targets ---

qemu: $(PROG_NAME) ## Run program on QEMU using default UART0 console
	qemu-system-riscv64 -nographic -machine sifive_u -kernel $(PROG_NAME) -bios none

# Walkie-Talkie Over TCP:
# This utilizes QEMU's character device redirection.
# -serial mon:stdio redirects UART0 to your terminal.
# -serial tcp:... redirects UART1 to a TCP socket.

qemu-talk-server: $(PROG_NAME) ## Start UART1 as a TCP server (Waits for client on 6969)
	@echo "Starting QEMU Server. Waiting for connection on port 6969..."
	qemu-system-riscv64 -nographic -machine sifive_u -kernel $(PROG_NAME) \
		-bios none -serial mon:stdio -serial tcp:localhost:6969,server,wait

qemu-talk-client: $(PROG_NAME) ## Start UART1 as a TCP client (Connects to localhost:6969)
	@echo "Starting QEMU Client. Connecting to localhost:6969..."
	qemu-system-riscv64 -nographic -machine sifive_u -kernel $(PROG_NAME) \
		-bios none -serial mon:stdio -serial tcp:localhost:6969

debug: $(PROG_NAME) ## Run on QEMU with GDB server enabled (Port :1234)
	qemu-system-riscv64 -M sifive_u -kernel $(PROG_NAME) -bios none -serial stdio -s -S

help: ## Show this help menu
	@echo "**************************************************"
	@echo "* RISC-V Baremetal Demo: SiFive U Board          *"
	@echo "**************************************************"
	@echo "* Usage: make [WALKIE_TALKIE_MODE=1] target      *"
	@echo "**************************************************"
	@echo ""
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
