OUTPUT_ARCH("riscv");

ENTRY(_start);

SECTIONS {
    . = PZ_CONFIG_MEM_START;

    .text : {
        *(.init)
        . = ALIGN(4K);
        *(.text)
        . = ALIGN(4K);
        PROVIDE(_text_end = .);
    }
    .rodata : {
        . = ALIGN(16);
        *(.srodata .srodata.*)
        . = ALIGN(16);
        *(.rodata .rodata.*)
        *(.eh_frame)
    }
    .data : {
        . = ALIGN(16);
        *(.sdata .sdata.*)
        . = ALIGN(16);
        *(.data .data.*)
    }
    .bss : {
        . = ALIGN(16);
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        . = ALIGN(16);
        *(.bss .bss.*)
        PROVIDE(_bss_end = .);
    }
    PROVIDE(_mem_end = .);
}

