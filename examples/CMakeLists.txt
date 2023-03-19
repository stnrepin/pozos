function(add_linker_script_to ELF_TARGET)
    target_link_options(
        ${ELF_TARGET}
        PRIVATE -T ${RISCV_D1_LDSCRIPT}
    )
endfunction()

function(make_bin_file NAME ELF_TARGET)
    add_custom_target(
        ${NAME}_bin
        ALL
        DEPENDS ${ELF_TARGET}
    )

    add_custom_command(
        TARGET ${NAME}_bin
        COMMAND ${CMAKE_OBJCOPY}
        ARGS
            --binary-architecture=riscv64 --strip-all -O binary
                $<TARGET_FILE:${ELF_TARGET}>
                $<TARGET_FILE_DIR:${ELF_TARGET}>/${NAME}.bin
    )
endfunction()

foreach(project_name ${EXAMPLE_PROJECTS})
    message(STATUS "Load project \"${project_name}\"")

    add_subdirectory(${project_name})
    message(STATUS ${SRC})

    add_executable(${project_name}_elf ${SRC})
    target_link_libraries(${project_name}_elf pozos_kern)

    add_linker_script_to(${project_name}_elf)
    make_bin_file(${project_name} ${project_name}_elf)
endforeach()
