cmake_minimum_required(VERSION 3.3.0)


add_library(Bootstrapper SHARED Bootstrapper.cpp)
set_target_properties(Bootstrapper PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "GeodeBootstrapper"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)

target_link_libraries(Bootstrapper PRIVATE "-framework CoreFoundation" filesystem)
