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
 * @file uart.h
 * @brief Hardware definitions for SiFive FU540 UART.
 * 
 * Provides base addresses, register offsets, and bit-field masks for 
 * the UART controllers used on the SiFive U board (QEMU sifive_u).
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>

/** 
 * @brief SiFive FU540 UART0 Base Address. 
 * Typically used for the main console output in QEMU.
 */
#define UART0_BASE ((uintptr_t)0x10010000)

/** 
 * @brief SiFive FU540 UART1 Base Address. 
 * Can be used for secondary communication or walkie-talkie mode.
 */
#define UART1_BASE ((uintptr_t)0x10011000)

/** @name UART Register Offsets */
/**@{*/
#define UART_REG_TXDATA 0x00  /**< Transmit data register */
#define UART_REG_RXDATA 0x04  /**< Receive data register */
#define UART_REG_TXCTRL 0x08  /**< Transmit control register */
#define UART_REG_RXCTRL 0x0C  /**< Receive control register */
#define UART_REG_IE     0x10  /**< Interrupt enable register */
#define UART_REG_IP     0x14  /**< Interrupt pending register */
#define UART_REG_DIV    0x18  /**< Baud rate divisor */
/**@}*/

/** @name TXDATA Bits */
/**@{*/
#define UART_TX_FULL (1U << 31) /**< Transmit FIFO full status bit */
/**@}*/

/** @name RXDATA Bits */
/**@{*/
#define UART_RX_EMPTY (1U << 31) /**< Receive FIFO empty status bit */
/**@}*/

/** @name TXCTRL/RXCTRL Bits */
/**@{*/
#define UART_TXEN 1 /**< Transmit enable bit */
#define UART_RXEN 1 /**< Receive enable bit */
/**@}*/

/**
 * @brief Macro for safe 64-bit register access.
 * 
 * @param base The base address of the UART controller.
 * @param off The register offset.
 * @return Volatile reference to the register.
 */
#define UART_REG(base, off) (*(volatile uint32_t *)((base) + (off)))

#endif /* UART_H */
