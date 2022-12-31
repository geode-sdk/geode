set(GEODE_CLI_MINIMUM_VERSION 1.0.5)

# for passing CLI through CMake arguments
if (DEFINED CLI_PATH)
    list(APPEND CMAKE_PROGRAM_PATH ${CLI_PATH}) 
endif()

# Find Geode CLI
if (NOT DEFINED GEODE_CLI)
    find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli)
endif()

# Check if CLI was found
if (GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
	message(STATUS "Unable to find Geode CLI")
else()
    # `geode --version` returns `geode x.x.x\n` so gotta do some wacky shit
    execute_process(
        COMMAND ${GEODE_CLI} --version
        OUTPUT_VARIABLE GEODE_CLI_VERSION
    )
    # Remove trailing newline
    string(STRIP ${GEODE_CLI_VERSION} GEODE_CLI_VERSION)
    # Remove program name
    string(REPLACE "geode " "" GEODE_CLI_VERSION ${GEODE_CLI_VERSION})

    # Need at least v1.0.5 (--shut-up arg in geode package resources)
    if (${GEODE_CLI_VERSION} VERSION_LESS ${GEODE_CLI_MINIMUM_VERSION})
        message(FATAL_ERROR
            "Found Geode CLI: ${GEODE_CLI}, however it is version ${GEODE_CLI_VERSION} "
            "while minimum required is version ${GEODE_CLI_MINIMUM_VERSION}. Please update: "
            "https://github.com/geode-sdk/cli/releases/latest"
        )
    endif()

    message(STATUS "Found Geode CLI: ${GEODE_CLI} (version ${GEODE_CLI_VERSION})")
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
    # Get DONT_INSTALL argument
    set(options DONT_INSTALL)
    cmake_parse_arguments(CREATE_GEODE_FILE "${options}" "" "" ${ARGN})

    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping creating geode file for ${proname}")
        return()
    endif()

    # what is this for
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json ${CMAKE_CURRENT_BINARY_DIR}/what.txt)
    set_target_properties(${proname} PROPERTIES CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(WARNING "create_geode_file called, but Geode CLI was not found - You will need to manually package the .geode files")
        return()
    endif()

    message(STATUS "Creating geode file for ${proname}")

    execute_process(
        COMMAND ${GEODE_CLI} package get-id ${CMAKE_CURRENT_SOURCE_DIR} --raw
        OUTPUT_VARIABLE MOD_ID
    )

    if (CREATE_GEODE_FILE_DONT_INSTALL)
        set(INSTALL_ARG "")
    else()
        set(INSTALL_ARG "--install")
    endif()

    set_target_properties(${proname} PROPERTIES PREFIX "")
    set_target_properties(${proname} PROPERTIES OUTPUT_NAME ${MOD_ID})
    add_custom_target(${proname}_PACKAGE ALL
        DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
        COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} --binary $<TARGET_FILE:${proname}> --output $<TARGET_FILE_DIR:${proname}>/${proname}.geode ${INSTALL_ARG}
        VERBATIM USES_TERMINAL
    )
endfunction()

function(package_geode_resources proname src dest)
    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping packaging resources from ${src} into ${dest}")
        return()
    endif()

    message(STATUS "Packaging resources from ${src} into ${dest}")

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(WARNING
            "package_geode_resources called, but Geode CLI was "
            "not found - You will need to manually package the resources"
        )
    else()

        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname}
            COMMAND ${GEODE_CLI} package resources ${src} ${dest}
            VERBATIM USES_TERMINAL
        )

    endif()
endfunction()

function(package_geode_resources_now proname src dest header_dest)
    if (GEODE_DISABLE_CLI_CALLS)
        message(WARNING
            "package_geode_resources_now called, but GEODE_DISABLE_CLI_CALLS 
            is set to true - Faking output result in case you only wish to 
            analyze the project statically, do not expect built project to 
            function properly"
        )
        set(HEADER_FILE
            "#include <unordered_map>\n\n"
            "static const std::unordered_map<std::string, std::string> "
            "LOADER_RESOURCE_HASHES {}\;\n"
        )
        file(WRITE ${header_dest} ${HEADER_FILE})
        message(STATUS "Wrote fake resource hashes to ${header_dest}")
        return()
    endif()

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(FATAL_ERROR
            "package_geode_resources_now called, but Geode CLI "
            "was not found - Please install Geode CLI from "
            "https://github.com/geode-sdk/cli/releases/latest"
        )
        return()
    endif()
    
    message(STATUS "Packaging resources now from ${src} into ${dest}")

    execute_process(
        COMMAND ${GEODE_CLI} package resources ${src} ${dest} --shut-up
        RESULT_VARIABLE GEODE_PACKAGE_RES
    )

    if (NOT GEODE_PACKAGE_RES EQUAL "0")
        message(FATAL_ERROR
            "Command \"${GEODE_CLI} package resources ${src} ${dest}\" returned "
            "${GEODE_PACKAGE_RES} - Expected 0"
        )
    endif()

    file(GLOB RESOURCE_FILES "${dest}/*.*")

    set(HEADER_FILE
        "#include <unordered_map>\n\n"
        "static const std::unordered_map<std::string, std::string> "
        "LOADER_RESOURCE_HASHES {\n"
        # "#include <vector>\n\n"
        # "static const std::vector<std::string> "
        # "LOADER_RESOURCE_FILES {\n"
    )

    list(APPEND HASHED_EXTENSIONS ".png")
    list(APPEND HASHED_EXTENSIONS ".mp3")
    list(APPEND HASHED_EXTENSIONS ".ogg")

    foreach(file ${RESOURCE_FILES})
        cmake_path(GET file FILENAME FILE_NAME)
        get_filename_component(FILE_EXTENSION ${file} EXT)
        list(FIND HASHED_EXTENSIONS "${FILE_EXTENSION}" FILE_SHOULD_HASH)

        if (NOT FILE_NAME STREQUAL ".geode_cache" AND NOT FILE_SHOULD_HASH EQUAL -1)
            
            file(SHA256 ${file} COMPUTED_HASH)
            list(APPEND HEADER_FILE "\t{ \"${FILE_NAME}\", \"${COMPUTED_HASH}\" },\n")

            # list(APPEND HEADER_FILE "\t\"${FILE_NAME}\",\n")

        endif()

    endforeach()

    list(APPEND HEADER_FILE "}\;\n")

    file(WRITE ${header_dest} ${HEADER_FILE})
    message(STATUS "Wrote resource hashes to ${header_dest}")

endfunction()
