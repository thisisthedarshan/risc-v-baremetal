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
 * @file helpers.c
 * @brief Implementation of memory-mapped UART utilities.
 * 
 * This file contains logic for formatted output and register access
 * for the SiFive FU540 UART controllers.
 */

#include "helpers.h"
#include "uart.h"

void uart_init(uintptr_t base) {
    /**
     * @brief Enable both transmission and reception on the given UART.
     * 
     * Sets the TXEN and RXEN bits in the CTRL registers.
     */
    UART_REG(base, UART_REG_TXCTRL) |= UART_TXEN;
    UART_REG(base, UART_REG_RXCTRL) |= UART_RXEN;
}

int uart_getc(uintptr_t base) {
    /**
     * @brief Reads a single byte from the RX FIFO.
     * 
     * If bit 31 is set, the FIFO is empty.
     */
    uint32_t val = UART_REG(base, UART_REG_RXDATA);
    if (val & UART_RX_EMPTY) {
        return -1;
    }
    return (int)(val & 0xFF);
}

void print_char(char c, uint8_t uart_no) {
    /**
     * @brief Waits for the TX FIFO to have space, then write the byte.
     * 
     * Uses uintptr_t for 64-bit address safety to avoid warnings.
     */
    uintptr_t base = (uart_no == 0) ? UART0_BASE : UART1_BASE;
    
    // Wait until TX FIFO is not full (bit 31 is 0)
    while (UART_REG(base, UART_REG_TXDATA) & UART_TX_FULL);
    
    // Write character
    UART_REG(base, UART_REG_TXDATA) = (uint8_t)c;
}

void print_string(const char *s, uint8_t uart_no) {
    /**
     * @brief Iterates through characters and writes each to the UART.
     */
    while (*s) {
        print_char(*s++, uart_no);
    }
}

void print_decimal(int64_t n, uint8_t uart_no) {
    /**
     * @brief Converts an integer to a string in reverse and prints it.
     */
    if (n == 0) {
        print_char('0', uart_no);
        return;
    }
    if (n < 0) {
        print_char('-', uart_no);
        n = -n;
    }
    
    char buf[20];
    int i = 0;
    while (n > 0) {
        buf[i++] = (n % 10) + '0';
        n /= 10;
    }
    // Reverse printing from buffer
    while (i > 0) {
        print_char(buf[--i], uart_no);
    }
}

void print_hex(uint64_t n, uint8_t uart_no) {
    /**
     * @brief Prints a 64-bit integer as a 16-digit hexadecimal.
     */
    const char *hex_chars = "0123456789ABCDEF";
    print_string("0x", uart_no);
    for (int i = 60; i >= 0; i -= 4) {
        print_char(hex_chars[(n >> i) & 0xF], uart_no);
    }
}

void print_binary(uint64_t n, uint8_t uart_no) {
    /**
     * @brief Prints a 64-bit integer bit-by-bit with byte separators.
     */
    print_string("0b", uart_no);
    for (int i = 63; i >= 0; i--) {
        print_char((n & (1ULL << i)) ? '1' : '0', uart_no);
        // Add an underscore separator every 8 bits for readability
        if (i > 0 && i % 8 == 0) {
            print_char('_', uart_no);
        }
    }
}
