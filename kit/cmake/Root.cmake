cmake_minimum_required(VERSION 3.13.4)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GEODE_SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
set(GEODE_INCLUDE_DIR ${GEODE_SDK_DIR}/include)

include(${CMAKE_CURRENT_LIST_DIR}/GeodeFile.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/LinkLoader.cmake)

if (NOT DEFINED GEODE_TARGET_PLATFORM)
	if(APPLE)
		set(GEODE_TARGET_PLATFORM "MacOS")
	elseif(WIN32)
		set(GEODE_TARGET_PLATFORM "Win32")
	else()
		message(FATAL_ERROR "Unable to detect platform, please set GEODE_TARGET_PLATFORM in the root CMake file.")
	endif()
endif()

if (GEODE_BUILD_CODEGEN)
	add_definitions(-DGEODE_EXPORTING_CODEGEN)

	set(GEODE_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/gen)

	file(MAKE_DIRECTORY ${GEODE_CODEGEN_DIR})
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

if (GEODE_BUILD_CODEGEN)
	if (GEODE_IMPLICIT_ENTRY)
		set(IMPLICIT_PATH ${GEODE_INCLUDE_DIR}/implicitEntry.cpp)
	else()
		set(IMPLICIT_PATH "")
	endif()
	set_source_files_properties(${GEODE_CODEGEN_DIR}/Source.cpp PROPERTIES GENERATED 1)
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES}
		${GEODE_CODEGEN_DIR}/Source.cpp
		${IMPLICIT_PATH}
	)
	# set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	# target_link_options(${PROJECT_NAME} PUBLIC -dead_strip)
else()
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
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

if (GEODE_BUILD_CODEGEN)
	add_subdirectory(${GEODE_SDK_DIR}/kit/gen)
	
	target_include_directories(${PROJECT_NAME} PUBLIC
		${CMAKE_BINARY_DIR}/
	)
endif()

target_compile_definitions(${PROJECT_NAME}
	PRIVATE -DPROJECT_NAME=${PROJECT_NAME}
	# PRIVATE "-D__FILENAME__='\"$(subst${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'"
)

if (NOT DEFINED GEODE_NO_LINK_LOADER)
	geode_link_bin(${PROJECT_NAME} "${GEODE_SDK_DIR}/bin")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
	${GEODE_INCLUDE_DIR}
	${GEODE_INCLUDE_DIR}/helpers
	${GEODE_INCLUDE_DIR}/base
	${GEODE_INCLUDE_DIR}/c++stl
	${GEODE_INCLUDE_DIR}/hook
	${GEODE_INCLUDE_DIR}/loader
	${GEODE_INCLUDE_DIR}/utils
	${GEODE_INCLUDE_DIR}/meta
	${GEODE_INCLUDE_DIR}/cocos/
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/include
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/support/zip_support
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/kazmath/include
	${GEODE_INCLUDE_DIR}/cocos/extensions
	${GEODE_INCLUDE_DIR}/fmod
	${GEODE_INCLUDE_DIR}/fmt/include
	${INCLUDE_DIRECTORIES}
)

if (DEFINED "${LINK_DIRECTORIES}")
target_link_directories(${PROJECT_NAME} 
	${LINK_DIRECTORIES}
)
endif()

if (GEODE_BUILD_CODEGEN)
	target_link_libraries(${PROJECT_NAME} 
		fmt
	)
endif()

target_link_libraries(${PROJECT_NAME}
	${LINK_LIBRARIES}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC ${GEODE_INCLUDE_DIR}/Geode.hpp)

if (NOT DEFINED GEODE_NO_GEODE_FILE)
	create_geode_file(${PROJECT_NAME})
endif()
