set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "S;s")
set(CMAKE_LINKER ld.lld)
set(CMAKE_OBJCOPY llvm-objcopy)

set (CMAKE_C_LINKER_WRAPPER_FLAG "-Xlinker" " ")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(
    COMMON_COMPILER_OPTIONS
        -mno-relax
        -nostdlib
        -ffreestanding
)
