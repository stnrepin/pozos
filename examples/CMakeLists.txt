function(add_linker_script_to elf_target)
    target_link_options(
        ${elf_target}
        PRIVATE -T ${LDSCRIPT}
    )
endfunction()

function(make_bin_file name elf_target)
    add_custom_target(
        ${name}_bin
        ALL
        DEPENDS ${elf_target}
    )

    add_custom_command(
        TARGET ${name}_bin
        COMMAND ${CMAKE_OBJCOPY}
        ARGS
            --binary-architecture=${CMAKE_SYSTEM_PROCESSOR} --strip-all
            -O binary
            $<TARGET_FILE:${elf_target}>
            $<TARGET_FILE_DIR:${elf_target}>/${name}.bin
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
