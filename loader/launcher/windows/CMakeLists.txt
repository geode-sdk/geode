cmake_minimum_required(VERSION 3.3.0)

add_library(ProxyLoader SHARED proxyLoader.c)
target_compile_features(ProxyLoader PUBLIC cxx_std_17)
set_target_properties(ProxyLoader PROPERTIES
	PREFIX ""
	OUTPUT_NAME "XInput9_1_0"
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)
add_library(fake-geode-loader SHARED fakeGeode.c)
set_target_properties(fake-geode-loader PROPERTIES OUTPUT_NAME "Geode")
target_link_libraries(ProxyLoader PRIVATE fake-geode-loader)

add_executable(Updater Updater.cpp)
target_compile_features(Updater PUBLIC cxx_std_17)
set_target_properties(Updater PROPERTIES
	PREFIX ""
	OUTPUT_NAME "GeodeUpdater"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)
target_link_libraries(Updater PUBLIC ghc_filesystem)

if (MSVC)
	include(CheckLinkerFlag)
	check_linker_flag(CXX /NOEXP SUPPORTS_NOEXP)
	if (SUPPORTS_NOEXP)
		target_link_options(fake-geode-loader PRIVATE /NOEXP)
		target_link_options(ProxyLoader PRIVATE /NOEXP)
	endif()

	target_link_options(fake-geode-loader PRIVATE /DEBUG:NONE)
	target_link_options(ProxyLoader PRIVATE /NOIMPLIB /DEBUG:NONE)
	target_link_options(Updater PRIVATE /DEBUG:NONE)

	if (DEFINED ENV{GEODE_CI})
		target_link_options(fake-geode-loader PRIVATE /INCREMENTAL:NO)
		target_link_options(ProxyLoader PRIVATE /INCREMENTAL:NO)
		target_link_options(Updater PRIVATE /INCREMENTAL:NO)
	endif()
endif()
