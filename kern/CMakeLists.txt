add_subdirectory(arch/riscv64)

add_library(
    pozos_kern
        init.c
        mm.c
        sched.c
        printf.c
        uart.c
        console.c
)
target_compile_definitions(
    pozos_kern
    PRIVATE PRINTF_INCLUDE_CONFIG_H
)
target_include_directories(
    pozos_kern
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(pozos_kern pz_arch_riscv64)
