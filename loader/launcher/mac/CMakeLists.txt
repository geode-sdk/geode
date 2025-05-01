cmake_minimum_required(VERSION 3.21)


add_library(GeodeBootstrapper SHARED Bootstrapper.cpp)
target_compile_features(GeodeBootstrapper PUBLIC cxx_std_17)
set_target_properties(GeodeBootstrapper PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "GeodeBootstrapper"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)

target_link_libraries(GeodeBootstrapper PRIVATE)

add_library(FakeGeode SHARED FakeGeode.cpp)
target_compile_features(FakeGeode PUBLIC cxx_std_17)
set_target_properties(FakeGeode PROPERTIES
	PREFIX "" 
	OUTPUT_NAME "Geode"
)

set_target_properties(GeodeBootstrapper PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(GeodeBootstrapper PROPERTIES INSTALL_RPATH "./")

set_target_properties(FakeGeode PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(FakeGeode PROPERTIES INSTALL_RPATH "./")

target_link_libraries(GeodeBootstrapper PRIVATE FakeGeode)
