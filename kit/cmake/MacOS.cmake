add_definitions(-DCC_TARGET_OS_MAC)
set(CMAKE_SYSTEM_NAME MacOS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Wno-deprecated -O2 -Os -fdata-sections -ffunction-sections")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -dead_strip")

include_directories(
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/mac
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/mac
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/mac/OGLES
)

function(geode_link_bin proname bin_repo_directory)
	target_link_libraries(
	    ${proname}
	    "${bin_repo_directory}/macos/Geode.dylib"
	)
	target_include_directories(
	    ${proname} PUBLIC
	    "${bin_repo_directory}/macos"
	)
endfunction()

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND strip -x $<TARGET_FILE:${PROJECT_NAME}>
#     VERBATIM USES_TERMINAL
# )
