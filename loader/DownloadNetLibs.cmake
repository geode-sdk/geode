if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=8b4a965cfb3bede3b377ce174e58da195b115e296b814457e3cf4f13ca11ed56")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=d83d968e709ac373a985170cfb2fb7477678f79e6afa307b864ced14e96e0be5")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=9de01260210dc8141881de6cd313fadf8725c04c9a8ab6e3453dc828c6918dda")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=f2274df2db9a540be961646f5400df394cef6b86a75b127fd131ff97b0feb7cf")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=5c072b264c8a82b2bd6b64bd9b4f8c0ea6bf0a09650c70bdd076ff54b0ddf408")
endif()

set(net_libs_version "8.19.0-7")
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