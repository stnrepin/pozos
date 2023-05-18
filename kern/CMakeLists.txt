add_subdirectory(arch/riscv64)

add_library(
    pozos_kern
        console.c
        init.c
        kalloc.c
        mm.c
        printf.c
        sched.c
        string.c
        uart.c
)
target_compile_definitions(
    pozos_kern
    PRIVATE
        PRINTF_INCLUDE_CONFIG_H
)
target_include_directories(
    pozos_kern
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(pozos_kern pz_arch_riscv64)
