if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(net_libs_plat "ios")
	set(net_libs_hash "SHA256=1f04d932067c1d75d2284e226acfb3b89a962915e4e976263d02ddb931b37dd4")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set(net_libs_plat "macos")
	set(net_libs_hash "SHA256=d814f317abfa95b583f1d37120e686d8cb2a089a511e1372dcdd7e462936bf09")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set(net_libs_plat "windows")
	set(net_libs_hash "SHA256=60ca9a0970fb5ebde78b68d20ef8817c927281422f3448ef35defccf30988a87")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set(net_libs_plat "android32")
	set(net_libs_hash "SHA256=263d4a01e30523b5373ad156626359e28874fbee87ff08f18338186c5821a376")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set(net_libs_plat "android64")
	set(net_libs_hash "SHA256=8546de2a9c3f0208e8e024fba60812e92d1fc1f23d1904b7ac1d3cc78953238b")
endif()

set(net_libs_version "8.22.0-1")
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

CPMAddPackage("gh:geode-sdk/net_libs@${net_libs_version}")
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
