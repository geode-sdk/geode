cmake_minimum_required(VERSION 3.13.4)

find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli)

if(NOT GEODE_CLI)
    message(STATUS "Unable to find Geode CLI - You will need to manually package the .geode files")
else()
    message(STATUS "Found Geode CLI: ${GEODE_CLI}")
endif()
    
function(create_geode_file proname)
    message(STATUS "Creating geode file")

    if(NOT GEODE_CLI)
        message(WARNING "create_geode_file called, but Geode CLI was not found - You will need to manually package the .geode files")
    else()

        if (GEODE_DONT_AUTO_INSTALL)
            set(INSTALL_FLAGS "")
            message(STATUS "Not installing after build")
        else()
            set(INSTALL_FLAGS --install)
            message(STATUS "Installing ${proname} in geode/mods after build")
        endif()

        add_custom_command(
            TARGET ${proname} POST_BUILD
            COMMAND ${GEODE_CLI} pkg ${CMAKE_CURRENT_SOURCE_DIR} $<TARGET_FILE_DIR:${proname}> $<TARGET_FILE_DIR:${proname}>/${proname}.geode ${INSTALL_FLAGS} --cached
            VERBATIM USES_TERMINAL
        )
    endif()

endfunction()
