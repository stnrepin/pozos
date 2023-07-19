#!/bin/bash

qemu-system-riscv64 -nographic -machine virt \
    -bios opensbi-riscv64-generic-fw_dynamic.bin \
    -kernel $1 -serial mon:stdio \
    ${@:2}
