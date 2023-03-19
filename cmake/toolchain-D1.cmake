set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION RISCV-D1)

set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "S;s")
set(CMAKE_LINKER ld.lld)
set(CMAKE_OBJCOPY llvm-objcopy)

set(
    RISCV_OPTIONS
        -mno-relax
        -nostdlib
        -ffreestanding
        --target=riscv64
)

add_compile_options(
    ${RISCV_OPTIONS}
)

add_link_options(
    ${RISCV_OPTIONS}
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
