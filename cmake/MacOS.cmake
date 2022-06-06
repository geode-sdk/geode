add_definitions(-DCC_TARGET_OS_MAC)
set(CMAKE_SYSTEM_NAME MacOS)

target_compile_options(${PROJECT_NAME} PUBLIC -fms-extensions -Wno-deprecated -Wno-ignored-attributes -Os -flto -fvisibility=internal)
set(CMAKE_OSX_ARCHITECTURES x86_64)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9) # did i tell im lazy


include_directories(
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/mac
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/mac
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/mac/OGLES
)

target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
if(NOT ${GEODE_NO_LOADER})
	target_link_libraries(
	    ${PROJECT_NAME}
	    "${GEODE_SDK_PATH}/../macos/Geode.dylib"
	)
endif()

if (NOT ${GEODE_DO_CODEGEN})
	target_include_directories(
		${PROJECT_NAME} PRIVATE
		"${GEODE_SDK_PATH}/../macos/"
	)
endif()


macro(link_prebuilts project)
	target_link_libraries(${project} 
		"${GEODE_SDK_DIR}/lib/macos/Geometry Dash"
	)
endmacro()

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND strip -x $<TARGET_FILE:${PROJECT_NAME}>
#     VERBATIM USES_TERMINAL
# )
