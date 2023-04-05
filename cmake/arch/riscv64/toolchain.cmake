set(toolchain_dir "${CMAKE_CURRENT_LIST_DIR}/../..")
include(${toolchain_dir}/toolchain-common.cmake)

set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(
    riscv_options
        ${COMMON_COMPILER_OPTIONS}
        --target=riscv64
)

add_compile_options(
    ${riscv_options}
)

add_link_options(
    ${riscv_options}
)

