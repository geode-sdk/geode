set(GEODE_CLI_MINIMUM_VERSION 1.0.5)

# Find Geode CLI
if (NOT DEFINED GEODE_CLI OR GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
    find_program(GEODE_CLI NAMES geode.exe geode-cli.exe geode geode-cli PATHS ${CLI_PATH})
endif()

# Check if CLI was found
if (GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
	message(WARNING "Unable to find Geode CLI")
else()
    # `geode --version` returns `geode x.x.x\n` so gotta do some wacky shit
    execute_process(
        COMMAND ${GEODE_CLI} --version
        OUTPUT_VARIABLE GEODE_CLI_VERSION
        COMMAND_ERROR_IS_FATAL ANY
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

    # Cache version so it's available to other functions
    set(GEODE_CLI_VERSION "${GEODE_CLI_VERSION}" CACHE INTERNAL "GEODE_CLI_VERSION")

    message(STATUS "Found Geode CLI: ${GEODE_CLI} (version ${GEODE_CLI_VERSION})")
endif()

# Clear cache of mods being built so mods from earlier 
# configures dont appear on the list
set(GEODE_MODS_BEING_BUILT "" CACHE INTERNAL "GEODE_MODS_BEING_BUILT")

# todo: add EXTERNAL argument that takes a list of external mod ids
# current workaround is to manually append to GEODE_MODS_BEING_BUILT before 
# calling setup_geode_mod if the mod depends on external dependencies that 
# aren't being built
function(setup_geode_mod proname)
    # Get DONT_INSTALL argument
    set(options DONT_INSTALL)
    set(multiValueArgs EXTERNALS)
    cmake_parse_arguments(SETUP_GEODE_MOD "${options}" "" "${multiValueArgs}" ${ARGN})

    # Link Geode to the mod
    target_link_libraries(${proname} geode-sdk)

    if (ANDROID)
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            add_custom_command(
                TARGET "${proname}" POST_BUILD
                DEPENDS "${proname}"
                COMMAND $<$<CONFIG:release>:${CMAKE_STRIP}>
                ARGS -S $<TARGET_FILE:${proname}>
            )
        endif()
    endif()

    if (GEODE_DISABLE_CLI_CALLS)
        message("Skipping setting up geode mod ${proname}")
        return()
    endif()

    if(GEODE_CLI STREQUAL "GEODE_CLI-NOTFOUND")
        message(FATAL_ERROR
            "setup_geode_mod called, but Geode CLI was not found - "
            "Please install CLI: https://docs.geode-sdk.org/"
        )
        return()
    endif()

    # what is this for
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json ${CMAKE_CURRENT_BINARY_DIR}/what.txt)
    set_target_properties(${proname} PROPERTIES CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)

    message(STATUS "Setting up ${proname}")

    # Read mod.json
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mod.json" MOD_JSON)
    string(JSON MOD_ID GET "${MOD_JSON}" "id")
    string(JSON MOD_VERSION GET "${MOD_JSON}" "version")
    string(JSON TARGET_GEODE_VERSION GET "${MOD_JSON}" "geode")
    string(JSON MOD_HAS_API ERROR_VARIABLE MOD_DOESNT_HAVE_API GET "${MOD_JSON}" "api")
    string(JSON MOD_HAS_DEPS ERROR_VARIABLE MOD_DOESNT_HAVE_DEPS GET "${MOD_JSON}" "dependencies")

    string(REGEX REPLACE "([0-9]+\\.[0-9]+)\\.[0-9]+" "\\1" TARGET_GEODE_VERSION_SHORT ${TARGET_GEODE_VERSION})
    string(REGEX REPLACE "([0-9]+\\.[0-9]+)\\.[0-9]+" "\\1" GEODE_VERSION_SHORT ${GEODE_VERSION_FULL})

    if ("${TARGET_GEODE_VERSION_SHORT}" STREQUAL "${GEODE_VERSION_SHORT}")
        message(STATUS "Mod ${MOD_ID} is compiling for Geode version ${GEODE_VERSION_FULL}")
    else()
        message(FATAL_ERROR
            "Mod ${MOD_ID} is made for Geode version ${TARGET_GEODE_VERSION} but you have ${GEODE_VERSION_FULL} SDK installed. Please change the Geode version in your mod.json. "
        )
    endif()

    target_compile_definitions(${proname} PRIVATE GEODE_MOD_ID="${MOD_ID}")

    # Add this mod to the list of known externals mods
    list(APPEND GEODE_MODS_BEING_BUILT "${MOD_ID}:${MOD_VERSION}")
    # Ensure that the list of mods being built is global (persists between setup_geode_mod calls)
    set(GEODE_MODS_BEING_BUILT ${GEODE_MODS_BEING_BUILT} CACHE INTERNAL "GEODE_MODS_BEING_BUILT")

    # Add function arg externals to the list but don't cache that
    if (SETUP_GEODE_MOD_EXTERNALS)
        list(APPEND GEODE_MODS_BEING_BUILT ${SETUP_GEODE_MOD_EXTERNALS})
    endif()

    # For CLI >=v2.4.0, there's an option to disable updating index because 
    # Github Actions on Mac just returns 403 for no reason
    if (GEODE_DONT_UPDATE_INDEX AND (${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "2.4.0"))
        set(DONT_UPDATE_INDEX_ARG "--dont-update-index")
    else()
        set(DONT_UPDATE_INDEX_ARG "")
    endif()

    # Check dependencies using CLI
    if (${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "3.2.0")
        execute_process(
            COMMAND ${GEODE_CLI} project check ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${GEODE_MODS_BEING_BUILT} ${DONT_UPDATE_INDEX_ARG}
                --platform ${GEODE_TARGET_PLATFORM_SHORT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "2.0.0")
        message(WARNING "If you use platform-specific dependencies, upgrade Geode CLI to version 3.2.0 or greater!")
        execute_process(
            COMMAND ${GEODE_CLI} project check ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${GEODE_MODS_BEING_BUILT} ${DONT_UPDATE_INDEX_ARG}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "1.4.0")
        execute_process(
            COMMAND ${GEODE_CLI} package setup ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${GEODE_MODS_BEING_BUILT}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (MOD_HAS_DEPS)
        message(FATAL_ERROR
            "CLI is version ${GEODE_CLI_VERSION}, but using dependencies "
            "requires at least 1.4.0 - please update your CLI"
        )
    endif()
    
    # Check if --install should be passed
    if (SETUP_GEODE_MOD_DONT_INSTALL OR GEODE_DONT_INSTALL_MODS)
        message(STATUS "Skipping installing ${proname}")
        set(INSTALL_ARG "")
    else()
        set(INSTALL_ARG "--install")
    endif()

    # The lib binary should be passed only if some headers were provided
    if (MOD_HAS_API)
        message(STATUS "Including library & headers with ${proname}")
        set(HAS_HEADERS On)
    else()
        set(HAS_HEADERS Off)
    endif()

    if (GEODE_BUNDLE_PDB AND WIN32 AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"))
        if (HAS_HEADERS)
            add_custom_target(${proname}_PACKAGE ALL
                DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
                COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                    --binary $<TARGET_FILE:${proname}> $<TARGET_LINKER_FILE:${proname}> $<TARGET_PDB_FILE:${proname}>
                    --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.geode
                    ${INSTALL_ARG} ${PDB_ARG}
                VERBATIM USES_TERMINAL
            )
        else()
            add_custom_target(${proname}_PACKAGE ALL
                DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
                COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                    --binary $<TARGET_FILE:${proname}> $<TARGET_PDB_FILE:${proname}>
                    --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.geode
                    ${INSTALL_ARG} ${PDB_ARG}
                VERBATIM USES_TERMINAL
            )
        endif()
    elseif (HAS_HEADERS AND WIN32)
        # this adds the .lib file on windows, which is needed for linking with the headers
        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
            COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                --binary $<TARGET_FILE:${proname}> $<TARGET_LINKER_FILE:${proname}>
                --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.geode
                ${INSTALL_ARG}
            VERBATIM USES_TERMINAL
        )
    else()
        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
            COMMAND ${GEODE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                --binary $<TARGET_FILE:${proname}>
                --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.geode
                ${INSTALL_ARG}
            VERBATIM USES_TERMINAL
        )
    endif()

    # Add dependency dir to include path
    if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/geode-deps")

        file(GLOB dirs ${CMAKE_CURRENT_BINARY_DIR}/geode-deps/*)

        set(libs_to_link "")

        # Iterate dependency directories
        foreach(dir ${dirs})
            if (IS_DIRECTORY ${dir})
                
                # v1.4.1 fixes optional dependencies
                if (${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "1.4.1")
                    # Read dep info
                    file(READ "${dir}/geode-dep-options.json" DEP_JSON)
                    string(JSON required GET "${DEP_JSON}" "required")

                    # If this is not a required dependency, don't link it
                    if (NOT ${required})
                        continue()
                    endif()
                else()
                    message(WARNING
                        "You are using CLI v1.4.0, which has a bug with optional "
                        "dependencies - update to v1.4.1 if you want to use "
                        "optional dependencies!"
                    )
                endif()

                # Otherwise add all .libs or whatever the platform's library type is
                if (WIN32 OR LINUX)
                    file(GLOB libs ${dir}/*.lib)
                    list(APPEND libs_to_link ${libs})
                elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
                    file(GLOB libs ${dir}/*.ios.dylib)
                    list(APPEND libs_to_link ${libs})
                elseif (APPLE)
                    file(GLOB libs ${dir}/*.dylib)
                    file(GLOB ios_libs ${dir}/*.ios.dylib)
                    list(REMOVE_ITEM libs ${ios_libs})
                    list(APPEND libs_to_link ${libs})
                elseif (ANDROID)
                    if (CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
                        file(GLOB libs ${dir}/*.android64.so)
                    else()
                        file(GLOB libs ${dir}/*.android32.so)
                    endif()
                    list(APPEND libs_to_link ${libs})
                else()
                    message(FATAL_ERROR "Library extension not defined on this platform")
                endif()

            endif()
        endforeach()

        # Link libs
        target_include_directories(${proname} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/geode-deps")
        target_link_libraries(${proname} ${libs_to_link})
        
    endif()

    # Add package target + make output name the mod id
    set_target_properties(${proname} PROPERTIES PREFIX "")
    if (DEFINED GEODE_MOD_BINARY_SUFFIX)
        set_target_properties(${proname} PROPERTIES SUFFIX ${GEODE_MOD_BINARY_SUFFIX})
    endif()
    set_target_properties(${proname} PROPERTIES OUTPUT_NAME ${MOD_ID})
endfunction()

function(create_geode_file proname)
    # todo: deprecate at some point ig
    # message(DEPRECATION
    #     "create_geode_file has been replaced with setup_geode_mod - "
    #     "please replace the function call"
    # )

    # forward all args
    setup_geode_mod(${proname} ${ARGN})
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
        COMMAND_ERROR_IS_FATAL ANY
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

    # yeah don't think we need to check too many stuff
    # list(APPEND HASHED_EXTENSIONS ".png")
    # list(APPEND HASHED_EXTENSIONS ".mp3")
    # list(APPEND HASHED_EXTENSIONS ".ogg")
    list(APPEND HASHED_TEXT_EXTENSIONS ".md")

    foreach(file ${RESOURCE_FILES})
        cmake_path(GET file FILENAME FILE_NAME)
        get_filename_component(FILE_EXTENSION ${file} EXT)

        list(FIND HASHED_EXTENSIONS "${FILE_EXTENSION}" FILE_SHOULD_HASH)

        if (NOT FILE_NAME STREQUAL ".geode_cache" AND NOT FILE_SHOULD_HASH EQUAL -1)
            
            file(SHA256 ${file} COMPUTED_HASH)
            file(SIZE ${file} FILE_SIZE)
            message(STATUS "Hashed ${file} to ${COMPUTED_HASH} (${FILE_SIZE} bytes)")
            list(APPEND HEADER_FILE "\t{ \"${FILE_NAME}\", \"${COMPUTED_HASH}\" },\n")

            # list(APPEND HEADER_FILE "\t\"${FILE_NAME}\",\n")

        endif()

        list(FIND HASHED_TEXT_EXTENSIONS "${FILE_EXTENSION}" FILE_SHOULD_TEXT_HASH)

        if (NOT FILE_NAME STREQUAL ".geode_cache" AND NOT FILE_SHOULD_TEXT_HASH EQUAL -1)
            
            # create list of lines form the contens of a file
            file(STRINGS ${file} LINES)
            list(JOIN LINES "" JOINED)
            # compute hash of the lines
            string(LENGTH "${JOINED}" FILE_SIZE)
            string(SHA256 COMPUTED_HASH "${JOINED}")
            
            message(STATUS "Hashed ${file} to ${COMPUTED_HASH} (${FILE_SIZE} bytes)")
            list(APPEND HEADER_FILE "\t{ \"${FILE_NAME}\", \"${COMPUTED_HASH}\" },\n")

            # list(APPEND HEADER_FILE "\t\"${FILE_NAME}\",\n")

        endif()

    endforeach()

    list(APPEND HEADER_FILE "}\;\n")

    file(WRITE ${header_dest} ${HEADER_FILE})
    message(STATUS "Wrote resource hashes to ${header_dest}")

endfunction()
