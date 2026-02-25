if (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_names cares.lib libcurl.lib nghttp2.lib libcrypto.lib libssl.lib zs.lib zstd_static.lib)
else()
	set(net_libs_names libcares.a libcurl.a libnghttp2.a libcrypto.a libssl.a libz.a libzstd.a)
endif()

if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=811cb13a358eb270efd1165b92d058ff73f228298a1a02ebb6f48bad0964e429")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=4ff0d651c591382cfbe70f324612a52634a6a2cbe46ee90fe648356d2ec191ae")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=cad982ac627a727861e9a9e0049d6da3b398a1032a6209e384074200091696fd")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=9fdf7a49d65fe722271893b79345854800de0984b83a15cd728ecbcfba2e173c")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=0026b266da460c3b8409763469ac3788e85bc5488424c4efc9736923f54e09e4")
endif()

CPMAddPackage(
	NAME net_libs_bin
	VERSION "8.19.0-4_${net_libs_plat}"
	URL "https://github.com/geode-sdk/net_libs/releases/download/v8.19.0-4/curl-${net_libs_plat}.zip"
	URL_HASH ${net_libs_hash}
	DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${net_libs_bin_SOURCE_DIR}/include)
foreach(lib ${net_libs_names})
	target_link_libraries(${PROJECT_NAME} "${net_libs_bin_SOURCE_DIR}/${lib}")
endforeach()

CPMAddPackage("gh:geode-sdk/net_libs#b5b810c")
target_link_libraries(${PROJECT_NAME} ca-bundle)

if (WIN32)
	set(ZLIB_LIBRARY "${net_libs_bin_SOURCE_DIR}/zs.lib")
else()
	set(ZLIB_LIBRARY "${net_libs_bin_SOURCE_DIR}/libz.a")
endif()

set(ZLIB_INCLUDE_DIR "${net_libs_bin_SOURCE_DIR}/include")