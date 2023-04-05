cmake_minimum_required(VERSION 3.16)

project(pozos LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(EXAMPLE_PROJECTS "hello")
set(
    LDSCRIPT
        "${CMAKE_CURRENT_SOURCE_DIR}/kern/arch/${CMAKE_SYSTEM_PROCESSOR}/linker.ld"
)

add_compile_options(
    -Wall
    -Wextra
    -Werror
)

add_subdirectory(kern)
add_subdirectory(examples)
