# Tiny-ARM-Cortex-M4-kernel

# Project:
Attempting to create a mini kernel with basic functions: basic drivers, timer, a scheduling system, threads/tasks management,inter-process communication protocal, system calls, and basic memory managment. 

# Comments:
The project uses GNU toolchain and thus the Makefile, startup.s, linkerScript.ld, and the source code was manually put together. By forcing myself to use this open-source toolchain I was able to increase my understanding of the compilation process and the low level details of chip memory layout, general chip architecture, as well as how program is loaded into memory and how the program counter commences the program.

# Chip:
ARM Cortex-M4 (little endian version)

# Board:
STM32F11

# Toolchain used (GNU based):
arm-none-eabi (gcc, ld, GDB, size)

# Flash tools:
st-utils (CLI)

# Projet Components:
1) src: 
  source code and include/external-lib files. The kernel and its services are contained within this dir.

2) linkerscript.ld:
  describes the memory layout of the chip (vector table, program text, initialized data, bss, etc.) and instructs the linker on the structure of the output image file. This script creates meta-data, which helps the CPU navigate the binary image and start the program.

3) startup.s:
  Initializes the ARM chip and navigates to the start of the program. In this stage the program is copied from the ROM and into the RAM. Basic services such as interrupts, timers, etc. are initialized in this stage. 
  
4) Makefile:
  contains recipes for building the project. This file invokes GNU tools such as gcc, ld, GDB, and size.  

# Future goals
Write the bootloader for this specific board

