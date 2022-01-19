add_definitions(-DCC_TARGET_OS_MAC)
set(CMAKE_SYSTEM_NAME MacOS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Wno-deprecated")

include_directories(
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/platform/mac
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/mac
	${CACAO_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/mac/OGLES
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Cacao/Extern/MacOS/)
target_link_libraries(${PROJECT_NAME} libCacao.dylib)

find_program(OSXinj "osxinj")
if (OSXinj)
	add_custom_command(
		COMMAND sudo osxinj \"Geometry Dash\" *.dylib || true
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Inject target ${PROJECT_NAME}"
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/run0
	)

	add_custom_target(
		Inject ALL
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/run0
	)

	add_dependencies(Inject ${PROJECT_NAME})
endif()

macro(link_prebuilts project)
endmacro()
