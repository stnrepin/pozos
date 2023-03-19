add_library(
    pozos_kern
        start.S

        kernel.c
        printf.c
        uart.c
)
target_compile_definitions(
    pozos_kern
    PRIVATE PRINTF_INCLUDE_CONFIG_H
)
target_include_directories(
    pozos_kern
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
