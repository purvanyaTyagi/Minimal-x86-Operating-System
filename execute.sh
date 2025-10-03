#!/bin/bash
dd if=/dev/zero of=build/disk.img bs=512 count=2880
mkfs.fat -F 12 -n "NBOS" build/disk.img


nasm -f bin source/boot.asm -o build/boot.bin
nasm -f elf32 source/kernel/std/x86_protected_mode.asm -o build/kernel32_asm.o
#nasm -f bin source/kernel/std/x86_protected_mode.asm -o build/KERNEL32.bin
nasm -f bin source/kernel/std/kernload.asm -o build/kernload.bin
nasm -f obj -o build/kernel.o source/kernel/kernel.asm
nasm -f obj -o build/std/x86.o source/kernel/std/x86.asm

/usr/bin/watcom/binl64/wcc -4 -d3 -s -wx -ms -zl -zq -fo=build/kernel_c.o source/kernel/kernel.c
/usr/bin/watcom/binl64/wcc -4 -d3 -s -wx -ms -zl -zq -fo=build/std/stdlib.o source/kernel/std/stdlib.c 
gcc -ffreestanding -m32 -nostdlib -c source/kernel/std/kernel32.c -o build/kernel32.o -fno-pic
gcc -ffreestanding -m32 -nostdlib -c source/kernel/std/fesh_handler.c -o build/fesh_handler.o -fno-pic

/usr/bin/watcom/binl64/wlink NAME build/kernel.bin FILE \{build/std/x86.o build/kernel.o build/std/stdlib.o build/kernel_c.o \} OPTION MAP = build/kernel_map.map @build/linker.lnk
sudo ld -T build/linker32.ld -m elf_i386 -o build/KERNEL32.bin build/kernel32.o build/kernel32_asm.o build/fesh_handler.o --oformat binary

dd if=build/boot.bin of=build/disk.img bs=512 seek=0 conv=notrunc

mcopy -i build/disk.img build/kernel.bin "::kernel.bin"
mcopy -i build/disk.img source/test.txt "::test.txt"
mcopy -i build/disk.img build/KERNEL32.bin "::KERNEL32.bin"
mcopy -i build/disk.img build/kernload.bin "::kernload.bin"

mdir -i build/disk.img ::
qemu-system-i386 -fda build/disk.img -monitor stdio 

