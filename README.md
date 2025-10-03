# Minimal 16-bit Operating System

This project is a lightweight operating system designed to run in **16-bit real mode** using QEMU with a virtual floppy disk. It demonstrates core OS concepts including bootloading, file I/O, and basic user interaction.  

## Highlights
- **Bootloader:** A compact 512-byte bootloader that initializes the system and loads the OS from a virtual floppy.  
- **Disk Access:** Supports reading and writing using a FAT16-like file system, enabling full use of the virtual floppy.  
- **Kernel Development:** The OS kernel is implemented in C, leveraging WozMon for streamlined coding, compilation, and testing.  
- **Terminal Interface:** Provides a minimal terminal that allows text input, displays stored files, and echoes user commands.  

## Getting Started
1. Compile the kernel and create a floppy image.  
2. Launch the OS in QEMU:  
   ```bash
   qemu-system-i386 -fda floppy.img

Followed this playlist by Nanobyte as the reference for this project

https://www.youtube.com/watch?v=9t-SPC7Tczc&list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN
