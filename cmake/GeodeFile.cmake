if (NOT DEFINED GEODE_CLI)
    find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli)
endif()

if (GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
	message(STATUS "Unable to find Geode CLI")
else()
    message(STATUS "Found Geode CLI: ${GEODE_CLI}")
endif()

function(create_geode_file proname)
    message(STATUS "Creating geode file")

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(WARNING "create_geode_file called, but Geode CLI was not found - You will need to manually package the .geode files")
    else()

        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname}
            COMMAND ${GEODE_CLI} pkg ${CMAKE_CURRENT_SOURCE_DIR} $<TARGET_FILE_DIR:${proname}> $<TARGET_FILE_DIR:${proname}>/${proname}.geode --install --cached
            VERBATIM USES_TERMINAL
        )
    endif()

endfunction()
