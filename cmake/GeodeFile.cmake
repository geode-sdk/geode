if (NOT DEFINED GEODE_CLI)
    find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli)
endif()

if (GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
	message(STATUS "Unable to find Geode CLI")
else()
    message(STATUS "Found Geode CLI: ${GEODE_CLI}")
endif()

function(create_geode_file_old proname)
    message(
        DEPRECATION
        "create_geode_file_old has been deprecated. "
        "Please update to the new (v1.x.x) version of Geode CLI."
    )

    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping creating geode file")
        return()
    endif()

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

function(create_geode_file proname)
    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping creating geode file for ${proname}")
        return()
    endif()

    message(STATUS "Creating geode file for ${proname}")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json ${CMAKE_CURRENT_BINARY_DIR}/what.txt)
    set_target_properties(${proname} PROPERTIES CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)

    execute_process(
        COMMAND ${GEODE_CLI} package get-id ${CMAKE_CURRENT_SOURCE_DIR} --raw
        OUTPUT_VARIABLE MOD_ID
    )

    set_target_properties(${proname} PROPERTIES PREFIX "")
    set_target_properties(${proname} PROPERTIES OUTPUT_NAME ${MOD_ID})

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(WARNING "create_geode_file called, but Geode CLI was not found - You will need to manually package the .geode files")
    else()
        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
            COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} --binary $<TARGET_FILE:${proname}> --output $<TARGET_FILE_DIR:${proname}>/${proname}.geode --install
            VERBATIM USES_TERMINAL
        )
    endif()
endfunction()

function(package_geode_resources proname src dest)
    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping packaging resources from ${src} into ${dest}")
        return()
    endif()

    message(STATUS "Packaging resources from ${src} into ${dest}")

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(WARNING "package_geode_resources called, but Geode CLI was not found - You will need to manually package the resources")
    else()

        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname}
            COMMAND ${GEODE_CLI} package resources ${src} ${dest}
            VERBATIM USES_TERMINAL
        )
    endif()
endfunction()
