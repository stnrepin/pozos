add_library(
    pz_arch_riscv64
        start.S
        console.c
        init.c
        mm.c
        utils.c
)

target_include_directories(
    pz_arch_riscv64
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
