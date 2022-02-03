add_definitions(-DCC_TARGET_OS_WIN32)
set(CMAKE_SYSTEM_NAME Win32)

include_directories(
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/win32
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/win32
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/win32/OGLES

	${GEODE_INCLUDE_DIR}/hook/geode-meta/include
	${GEODE_INCLUDE_DIR}/hook/geode-meta/include/x86
)

macro(link_prebuilts project)
	target_link_libraries(${project} 
		${GEODE_SDK_DIR}/lib/windows/libcocos2d.lib
		${GEODE_SDK_DIR}/lib/windows/libExtensions.lib
	)
endmacro()

link_prebuilts(${PROJECT_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_SIZEOF_VOID_P 4)
set(CMAKE_SYSTEM_PROCESSOR "i686")
set(CMAKE_GENERATOR_PLATFORM x86)
install(
	TARGETS ${PROJECT_NAME} 
	DESTINATION ${CMAKE_BINARY_DIR}/out/
)

function(geode_link_bin proname bin_repo_directory)

    target_link_libraries(
        ${proname}
        "${bin_repo_directory}/windows/geode.lib"
        "${bin_repo_directory}/windows/CCZipUtils.lib"
    )
    target_include_directories(
        ${proname} PUBLIC
        "${bin_repo_directory}"
    )

endfunction()
