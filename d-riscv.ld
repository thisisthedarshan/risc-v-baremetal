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

OUTPUT_ARCH( "riscv" )   /* Specify the target architecture as RISC-V */
ENTRY( _start )          /* Define the entry point of the program */

SECTIONS                 /* Begin section definition */
{
  /* Define the starting address and stack size */
  START_ADDRESS = 0x80000000;  /* Starting address for the program */
  STACK_SIZE = 0x800;           /* Size of the stack (2 KB) */

  /* Start address */
  . = START_ADDRESS;            /* Set the current location to the starting address */

  /* Text section */
  /* Text section */
  .text : { 
    *(.text.init)    /* Crucial: Ensure startup code comes first */
    *(.text)         /* Collect all other code sections */
  }
  
  /* Data sections */
  .gnu_build_id : { *(.note.gnu.build-id) } /* Collect build ID information for debugging */
  .data : { *(.data) }          /* Collect initialized global/static variables */
  .rodata : { *(.rodata) }      /* Collect read-only data (e.g., string literals) */
  .sdata : { *(.sdata) }        /* Collect small data sections */
  .debug : { *(.debug) }        /* Collect debugging information */

  /* Stack section */
  stack_bottom = .;             /* Mark the beginning of the stack */
  . += STACK_SIZE;              /* Allocate space for the stack */
  stack_top = .;                /* Mark the end of the stack */

  _end = .;                     /* Define a symbol _end marking the end of the memory layout */
}
