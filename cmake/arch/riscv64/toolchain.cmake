set(toolchain_dir "${CMAKE_CURRENT_LIST_DIR}/../..")
include(${toolchain_dir}/toolchain-common.cmake)

set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(PZ_CONFIG_MEM_START 0x84000000)

set(
    riscv_options
        ${COMMON_COMPILER_OPTIONS}
        -DPZ_CONFIG_MEM_START=${PZ_CONFIG_MEM_START}
        --target=riscv64
        -mcmodel=medany
        -fno-PIE
)

add_compile_options(
    ${riscv_options}
)

add_link_options(
    ${riscv_options}
    "LINKER:--defsym=PZ_CONFIG_MEM_START=${PZ_CONFIG_MEM_START}"
)

