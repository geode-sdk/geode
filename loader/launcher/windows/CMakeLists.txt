cmake_minimum_required(VERSION 3.3.0)

add_library(ProxyLoader SHARED proxyLoader.c)
set_target_properties(ProxyLoader PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "XInput9_1_0"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)

add_library(Bootstrapper SHARED Bootstrapper.cpp)
set_target_properties(ProxyLoader PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "GeodeBootstrapper"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)
