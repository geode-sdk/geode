if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=bfa29fc3a088db4729aa63e92ead6038b70300dca47263e1800290054677eb82")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=4ac3bff41af04af4c54214bfd1c3fd495126066a357ce032a591264e8bc21274")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=00f53c5745b6d8a7b920f28a4b6b67710920253e936658ea59e41cd399769813")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=c0d0bb69f6ce5f9318e4c66ee67e6ee4c511535b47e09a96433c8a4d63ebfad0")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=30f5bc248876bd516450d78300df290386bb4ea4c1b727e0b1f5135a101d46cc")
endif()

set(net_libs_version "8.22.0")
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
	set(ZSTD_LIBRARY "${net_libs_bin_SOURCE_DIR}/zstd_static.lib")
else()
	set(ZLIB_LIBRARY "${net_libs_bin_SOURCE_DIR}/libz.a")
	set(ZSTD_LIBRARY "${net_libs_bin_SOURCE_DIR}/libzstd.a")
endif()

set(ZLIB_INCLUDE_DIR "${net_libs_bin_SOURCE_DIR}/include")
set(ZSTD_INCLUDE_DIR "${net_libs_bin_SOURCE_DIR}/include")

# libraries needed for minizip
if (NOT TARGET zstd::libzstd_static)
	add_library(zstd::libzstd_static STATIC IMPORTED)
    set_target_properties(zstd::libzstd_static PROPERTIES
        IMPORTED_LOCATION "${ZSTD_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ZSTD_INCLUDE_DIR}"
    )

	if (NOT TARGET zstd::libzstd_shared)
		add_library(zstd::libzstd_shared ALIAS zstd::libzstd_static)
	endif()
	if (NOT TARGET zstd::libzstd)
		add_library(zstd::libzstd ALIAS zstd::libzstd_static)
	endif()
	set(ZSTD_FOUND TRUE CACHE BOOL "" FORCE)
endif()
if (NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB STATIC IMPORTED)
    set_target_properties(ZLIB::ZLIB PROPERTIES
        IMPORTED_LOCATION "${ZLIB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIR}"
    )
	set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
endif()
