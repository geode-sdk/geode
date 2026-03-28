if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=1065ef2adc191a82de3f1d69826c29f8b1526accb092bde37966a5467606962c")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=c91a60646b8b0b4d879edf1230006d430de72b84d8feee8151cbd9136241898e")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=478170d63fe6850e032e1e2de51ad744fa595aac40da5df65afe9996313bccb7")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=e113ffeab4bfcbc193977ae695b87e63cc8c1062dd20270321798e45ff2d350c")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=9ace6335c077d42e4bc4510feea35212d628dab54a4df49b6792d6ee8aa81909")
endif()

set(net_libs_version "8.19.0-9")
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
		${net_libs_bin_SOURCE_DIR}/ngtcp2.lib
		${net_libs_bin_SOURCE_DIR}/ngtcp2_crypto_ossl.lib
		${net_libs_bin_SOURCE_DIR}/nghttp3.lib
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
		${net_libs_bin_SOURCE_DIR}/libngtcp2.a
		${net_libs_bin_SOURCE_DIR}/libngtcp2_crypto_ossl.a
		${net_libs_bin_SOURCE_DIR}/libnghttp3.a
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