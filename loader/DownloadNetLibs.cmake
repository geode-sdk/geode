if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=ee5fcba96144ff3b7c96e1373b15574d2a163901d99626c6488f4857b60f7ac6")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=a42936fc75e120ecca9f35c307a345f037f37cf3582b40c4c6db5fb7f2d0b646")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=eaa9721635dbd4839a6f415acd003a3d43bd5f2080d3a68fb95681202f1c7761")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=158d26a3dafe279ba802f2139c62b358f41dd38138b11469a14d0b10ae5c1d23")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=6006cb982b2bb3e96cc4773f4e9ad1fdf09c1a7c59486e91300e129689364234")
endif()

set(net_libs_version "8.19.0-6")
CPMAddPackage(
	NAME net_libs_bin
	VERSION "${net_libs_version}_${net_libs_plat}"
	URL "https://github.com/geode-sdk/net_libs/releases/download/v${net_libs_version}/curl-${net_libs_plat}.zip"
	URL_HASH ${net_libs_hash}
	DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${net_libs_bin_SOURCE_DIR}/include)
if (WIN32)
	target_link_libraries(${PROJECT_NAME} 
		${net_libs_bin_SOURCE_DIR}/cares.lib
		${net_libs_bin_SOURCE_DIR}/libcurl.lib
		${net_libs_bin_SOURCE_DIR}/nghttp2.lib
		${net_libs_bin_SOURCE_DIR}/libcrypto.lib
		${net_libs_bin_SOURCE_DIR}/libssl.lib
		${net_libs_bin_SOURCE_DIR}/zs.lib
		${net_libs_bin_SOURCE_DIR}/zstd_static.lib
	)
else()
	target_link_libraries(${PROJECT_NAME}
		${net_libs_bin_SOURCE_DIR}/libcares.a
		${net_libs_bin_SOURCE_DIR}/libcurl.a
		${net_libs_bin_SOURCE_DIR}/libnghttp2.a
		${net_libs_bin_SOURCE_DIR}/libcrypto.a
		${net_libs_bin_SOURCE_DIR}/libssl.a
		${net_libs_bin_SOURCE_DIR}/libz.a
		${net_libs_bin_SOURCE_DIR}/libzstd.a
	)
endif()

CPMAddPackage("gh:geode-sdk/net_libs#b5b810c")
target_link_libraries(${PROJECT_NAME} ca-bundle)

if (WIN32)
	set(ZLIB_LIBRARY "${net_libs_bin_SOURCE_DIR}/zs.lib")
else()
	set(ZLIB_LIBRARY "${net_libs_bin_SOURCE_DIR}/libz.a")
endif()

set(ZLIB_INCLUDE_DIR "${net_libs_bin_SOURCE_DIR}/include")