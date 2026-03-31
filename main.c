/*
 * MIT License
 * 
 * Copyright (c) 2026 Darshan
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file main.c
 * @brief Main logic for RISC-V Baremetal SiFive U Demo.
 * 
 * Demonstrates basic UART formatted output and initiates a 
 * "Walkie-Talkie" loop for cross-UART communication.
 */

#include "helpers.h"
#include "uart.h"

/**
 * @brief Entry point for the Baremetal application.
 * 
 * Called after startup.s has initialized the stack.
 */
void main(void) {
    // Initialize both UART0 and UART1 controllers
    uart_init(UART0_BASE);
    uart_init(UART1_BASE);

    // Initial greeting on the main console (UART0)
    print_string("--- RISC-V Baremetal Demo: SiFive U ---\n\n", 0);

    /* --- Motivational Quote Section --- */
    print_string("Quote of the Day (UART0):\n", 0);
    print_string("\"The only way to do great work is to love what you do.\" - Steve Jobs\n\n", 0);

    /* --- Poem Section --- */
    print_string("A Small Poem (UART0):\n", 0);
    print_string("Code is poetry, logic is art,\n", 0);
    print_string("Registers and harts, playback every part.\n", 0);
    print_string("Baremetal whispers, a world so deep,\n", 0);
    print_string("Promises to hardware, we always keep.\n\n", 0);

    /* --- Formatted Output Demonstration --- */
    print_string("Number formatting tests:\n", 0);
    
    print_string("Decimal: ", 0);
    print_decimal(123456789, 0);
    print_char('\n', 0);

    print_string("Hexadecimal: ", 0);
    print_hex(0xCAFEBABE, 0);
    print_char('\n', 0);

    print_string("Binary: ", 0);
    print_binary(0xAA55, 0);
    print_char('\n', 0);

    print_string("\nUART1 Initialized. Sending test packet...\n", 1);
    print_string("Hello from UART1! It works.\n", 1);

    /* --- Walkie-Talkie Mode --- */
#ifdef WALKIE_TALKIE_MODE
    print_string("\n--- Entering Walkie-Talkie Mode ---\n", 0);
    print_string("Any input received on UART1 will be echoed to UART0.\n", 0);
    print_string("Use the TCP-based QEMU targets to test this interaction.\n\n", 0);

    /**
     * @brief Walkie-Talkie Loop
     * 
     * Consistently polls UART1 for incoming data. If data is found,
     * it is echoed to UART0 as a character.
     */
    while (1) {
        int c = uart_getc(UART1_BASE);
        if (c != -1) {
            // Echo character from UART1 to UART0
            print_char((char)c, 0);
            
            // If it's a carriage return, add a newline for comfort
            if (c == '\r') {
                print_char('\n', 0);
            }
        }
    }
#else
    print_string("\nNormal Demo Finished. Parking the hart...\n", 0);
    while (1) {
        // In a real baremetal app, we might use 'wfi' (Wait For Interrupt) 
        // to save power here, but for this demo, a simple loop is sufficient.
        __asm__ volatile ("wfi");
    }
#endif
}
