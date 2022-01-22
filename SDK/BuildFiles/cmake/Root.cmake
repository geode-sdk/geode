set(CMAKE_BUILD_TYPE RelWithDebugInfo)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GEODE_SDK_DIR ${CMAKE_SOURCE_DIR}/sdk/SDK)
set(GEODE_INCLUDE_DIR ${GEODE_SDK_DIR}/Include)

function(create_geode_file proname)
	add_custom_command(
		TARGET ${proname} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
			"${CMAKE_CURRENT_SOURCE_DIR}/mod.json"
			"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mod.json"
			"${srcs}"
	)
	add_custom_command(
		TARGET ${proname}
		COMMAND ${CMAKE_COMMAND} -E tar "cf" 
			"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${proname}.geode" --format=zip -- 
			"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mod.json"
			"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${proname}.dll"
			"${srcs}"
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
		COMMAND ${CMAKE_COMMAND} -E echo "Creating zip file -> ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${proname}.geode"
	)
endfunction()

if (NOT DEFINED GEODE_TARGET_PLATFORM)
	if(APPLE)
		set(GEODE_TARGET_PLATFORM "MacOS")
	elseif(WIN32)
		set(GEODE_TARGET_PLATFORM "Win32")
	else()
		message(FATAL_ERROR "Unable to detect platform, please set GEODE_TARGET_PLATFORM in the root CMake file.")
	endif()
endif()
set(GEODE_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Gen)

file(MAKE_DIRECTORY ${GEODE_CODEGEN_DIR})

if (DEFINED SOURCE_FILES)
	file(WRITE ${GEODE_CODEGEN_DIR}/Source.cpp "// this text shouldn't be visible") 
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} 	${GEODE_CODEGEN_DIR}/Source.cpp)
else()
	message(FATAL_ERROR "Please set SOURCE_FILES in the root CMake file.")
endif()

if("${GEODE_TARGET_PLATFORM}" STREQUAL "MacOS")
	include(${CMAKE_CURRENT_LIST_DIR}/MacOS.cmake)
elseif("${GEODE_TARGET_PLATFORM}" STREQUAL "Win32")
	include(${CMAKE_CURRENT_LIST_DIR}/Win32.cmake)
elseif("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS")
	include(${CMAKE_CURRENT_LIST_DIR}/iOS.cmake)
elseif("${GEODE_TARGET_PLATFORM}" STREQUAL "Android")
	include(${CMAKE_CURRENT_LIST_DIR}/Android.cmake)
else()
	message(FATAL_ERROR "Not supported platform, please specify from the following: MacOS, Win32, iOS, Android.")
endif()

add_subdirectory(${GEODE_SDK_DIR}/BuildFiles/gen)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE -DPROJECT_NAME=${PROJECT_NAME}
	# PRIVATE "-D__FILENAME__='\"$(subst${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_BINARY_DIR}/
	${GEODE_INCLUDE_DIR}/
	${GEODE_INCLUDE_DIR}/helpers
	${GEODE_INCLUDE_DIR}/base
	${GEODE_INCLUDE_DIR}/c++stl
	${GEODE_INCLUDE_DIR}/hook
	${GEODE_INCLUDE_DIR}/loader
	${GEODE_INCLUDE_DIR}/keybinds
	${GEODE_INCLUDE_DIR}/core/include/geode/core
	${GEODE_INCLUDE_DIR}/cocos/
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/include
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/support/zip_support
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/kazmath/include
	${GEODE_INCLUDE_DIR}/cocos/extensions
	${GEODE_INCLUDE_DIR}/fmod
	${GEODE_INCLUDE_DIR}/gen/fmt/include
	${INCLUDE_DIRECTORIES}
)

if (DEFINED "${LINK_DIRECTORIES}")
target_link_directories(${PROJECT_NAME} 
	${LINK_DIRECTORIES}
)
endif()
target_link_libraries(${PROJECT_NAME} 
	fmt
	${LINK_LIBRARIES}
)

if (NOT DEFINED GEODE_NO_GEODE_FILE)
	create_geode_file(${PROJECT_NAME})
endif()
