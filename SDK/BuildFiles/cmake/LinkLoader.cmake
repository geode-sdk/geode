cmake_minimum_required(VERSION 3.13.4)

function(geode_link_bin proname bin_repo_directory)

    if(WIN32)
        target_link_libraries(
            ${proname}
            "${bin_repo_directory}/windows/geode.lib"
            "${bin_repo_directory}/windows/CCZipUtils.lib"
        )
    else()
        message(FATAL_ERROR "geode_link_bin is unimplemented for this platform!")
    endif()

endfunction()
