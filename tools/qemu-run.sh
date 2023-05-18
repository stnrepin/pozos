#!/bin/bash

qemu-system-riscv64 -nographic -machine virt -bios none -kernel $1 -serial mon:stdio ${@:2}
