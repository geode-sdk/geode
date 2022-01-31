cmake_minimum_required(VERSION 3.13.4)

find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli)

if(GEODE_CLI-NOTFOUND)
    message(WARNING "Unable to find Geode CLI - You will need to manually package the .geode files")
else()
    message(STATUS "Found Geode CLI: ${GEODE_CLI}")
endif()
    
function(create_geode_file proname)

    if(GEODE_CLI-NOTFOUND)
        message(WARNING "create_geode_file called, but Geode CLI was not found - You will need to manually package the .geode files")
    else()
        add_custom_command(
            TARGET ${proname} POST_BUILD
            COMMAND ${GEODE_CLI} pkg ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${proname}.geode --install
            VERBATIM USES_TERMINAL
        )
    endif()

endfunction()
