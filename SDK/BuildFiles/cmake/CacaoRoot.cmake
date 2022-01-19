set(CMAKE_BUILD_TYPE RelWithDebugInfo)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED CACAO_TARGET_PLATFORM)
	if(APPLE)
		set(CACAO_TARGET_PLATFORM "MacOS")
	elseif(WIN32)
		set(CACAO_TARGET_PLATFORM "Win32")
	else()
		message(FATAL_ERROR "Unable to detect platform, please set CACAO_TARGET_PLATFORM in the root CMake file.")
	endif()
endif()

set(CACAO_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Gen)

file(MAKE_DIRECTORY ${CACAO_CODEGEN_DIR})
set(CACAO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Cacao/Include)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/Cacao/Source/Cacao.cpp ${CMAKE_SOURCE_DIR}/Cacao/Source/HandlerFixes.cpp)
endif()

if (DEFINED SOURCE_FILES)
	file(WRITE ${CACAO_CODEGEN_DIR}/Source.cpp "// this text shouldn't be visible") 
	add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} 	${CACAO_CODEGEN_DIR}/Source.cpp)
else()
	message(FATAL_ERROR "Please set SOURCE_FILES in the root CMake file.")
endif()

if("${CACAO_TARGET_PLATFORM}" STREQUAL "MacOS")
	include(${CMAKE_CURRENT_LIST_DIR}/CacaoMacOS.cmake)
elseif("${CACAO_TARGET_PLATFORM}" STREQUAL "Win32")
	include(${CMAKE_CURRENT_LIST_DIR}/CacaoWin32.cmake)
elseif("${CACAO_TARGET_PLATFORM}" STREQUAL "iOS")
	include(${CMAKE_CURRENT_LIST_DIR}/CacaoiOS.cmake)
elseif("${CACAO_TARGET_PLATFORM}" STREQUAL "Android")
	include(${CMAKE_CURRENT_LIST_DIR}/CacaoAndroid.cmake)
else()
	message(FATAL_ERROR "Not supported platform, please specify from the following: MacOS, Win32, iOS, Android.")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Cacao/BuildFiles/gen)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE -DPROJECT_NAME=${PROJECT_NAME}
	# PRIVATE "-D__FILENAME__='\"$(subst${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_BINARY_DIR}/
	${CACAO_INCLUDE_DIR}/
	${CACAO_INCLUDE_DIR}/helpers
	${CACAO_INCLUDE_DIR}/base
	${CACAO_INCLUDE_DIR}/old_stl
	${CACAO_INCLUDE_DIR}/hook
	${CACAO_INCLUDE_DIR}/lilac
	${CACAO_INCLUDE_DIR}/lilac/core/include/lilac/core
	${CACAO_INCLUDE_DIR}/cocos/
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/include
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/support/zip_support
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/kazmath/include

	${CACAO_INCLUDE_DIR}/cocos/extensions
	${CACAO_INCLUDE_DIR}/fmod
	${CACAO_INCLUDE_DIR}/gen/fmt/include
	${INCLUDE_DIRECTORIES}
)

target_link_directories(${PROJECT_NAME} 
	PRIVATE Cacao
	${LINK_DIRECTORIES}
)
target_link_libraries(${PROJECT_NAME} 
	Cacao
	fmt
	${LINK_LIBRARIES}
)