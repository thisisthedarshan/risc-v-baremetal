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
 * @file helpers.h
 * @brief Utility and formatted output functions for RISC-V Baremetal.
 * 
 * Provides functions for strings, numbers, and basic UART interaction.
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initialize the UART controller.
 * 
 * Enables both the transmitter and receiver.
 * 
 * @param base Base address of the UART controller.
 */
void uart_init(uintptr_t base);

/**
 * @brief Read a character from the UART controller.
 * 
 * Non-blocking read. Returns -1 if the FIFO is empty.
 * 
 * @param base Base address of the UART controller.
 * @return The character read or -1 if none available.
 */
int uart_getc(uintptr_t base);

/**
 * @brief Print a single character to a specific UART.
 * 
 * Blocking write. Waits until the FIFO is not full.
 * 
 * @param c The character to send.
 * @param uart_no The UART index (0 or 1).
 */
void print_char(char c, uint8_t uart_no);

/**
 * @brief Print a null-terminated string to a specific UART.
 * 
 * @param s The string to print.
 * @param uart_no The UART index (0 or 1).
 */
void print_string(const char *s, uint8_t uart_no);

/**
 * @brief Print a 64-bit integer in decimal format to a specific UART.
 * 
 * Handles negative numbers and zero correctly.
 * 
 * @param n The integer to print.
 * @param uart_no The UART index (0 or 1).
 */
void print_decimal(int64_t n, uint8_t uart_no);

/**
 * @brief Print a 64-bit integer in hexadecimal format to a specific UART.
 * 
 * Outputs with a '0x' prefix and leading zeros.
 * 
 * @param n The integer to print.
 * @param uart_no The UART index (0 or 1).
 */
void print_hex(uint64_t n, uint8_t uart_no);

/**
 * @brief Print a 64-bit integer in binary format to a specific UART.
 * 
 * Outputs with a '0b' prefix, including underscores as byte separators.
 * 
 * @param n The integer to print.
 * @param uart_no The UART index (0 or 1).
 */
void print_binary(uint64_t n, uint8_t uart_no);

#endif /* HELPERS_H */
