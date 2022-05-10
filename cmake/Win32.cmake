add_definitions(-DCC_TARGET_OS_WIN32)
set(CMAKE_SYSTEM_NAME Win32)

include_directories(
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/win32
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/win32
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/win32/zlib
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/win32/OGLES

	${GEODE_SDK_PATH}/include/hook/geode-meta/include
	${GEODE_SDK_PATH}/include/hook/geode-meta/include/x86
)

macro(link_prebuilts project)
	target_link_libraries(${project} 
		${GEODE_SDK_PATH}/link/libcocos2d.lib
		${GEODE_SDK_PATH}/link/libExtensions.lib
		${GEODE_SDK_PATH}/link/libcurl.lib
	)
endmacro()

link_prebuilts(${PROJECT_NAME})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_SIZEOF_VOID_P 4)
set(CMAKE_SYSTEM_PROCESSOR "i686")
if(MSVC)
	set(CMAKE_GENERATOR_PLATFORM x86)
endif()
install(
	TARGETS ${PROJECT_NAME} 
	DESTINATION ${CMAKE_BINARY_DIR}/out/
)

if(NOT ${GEODE_NO_LOADER})
target_link_libraries(
    ${PROJECT_NAME}
    "${GEODE_SDK_PATH}/../windows/geode.lib"
    "${GEODE_SDK_PATH}/../windows/CCZipUtils.lib"
)
endif()

if (NOT ${GEODE_DO_CODEGEN})
	target_include_directories(
		${PROJECT_NAME} PRIVATE
		"${GEODE_SDK_PATH}/../windows/"
	)
endif()
