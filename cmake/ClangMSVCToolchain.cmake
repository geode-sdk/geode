# When configuring CMake with a toolchain file against a top-level CMakeLists.txt,
# it will actually run CMake many times, once for each small test program used to
# determine what features a compiler supports.  Unfortunately, none of these
# invocations share a CMakeCache.txt with the top-level invocation, meaning they
# won't see the value of any arguments the user passed via -D.  Since these are
# necessary to properly configure MSVC in both the top-level configuration as well as
# all feature-test invocations, we set environment variables with the values so that
# these environments get inherited by child invocations.
function(init_user_prop prop)
  if(${prop})
    set(ENV{_${prop}} "${${prop}}")
  else()
    set(${prop} "$ENV{_${prop}}" PARENT_SCOPE)
  endif()
endfunction()

macro(cmake_getconf VAR)
  if(NOT ${VAR})
    set(${VAR} "$ENV{${VAR}}")
    if(${VAR})
      set(${VAR} "${${VAR}}" CACHE STRING "${VAR}")
      message(STATUS "Found ${VAR}: ${${VAR}}")
    else()
      message(FATAL_ERROR "Cannot determine \"${VAR}\"")
    endif()
  else()
    set(ENV{${VAR}} "${${VAR}}")
  endif()
endmacro()

function(generate_winsdk_vfs_overlay winsdk_include_dir output_path)
  set(include_dirs)
  file(GLOB_RECURSE entries LIST_DIRECTORIES true "${winsdk_include_dir}/*")
  foreach(entry ${entries})
    if(IS_DIRECTORY "${entry}")
      list(APPEND include_dirs "${entry}")
    endif()
  endforeach()

  file(WRITE "${output_path}"  "version: 0\n")
  file(APPEND "${output_path}" "case-sensitive: false\n")
  file(APPEND "${output_path}" "roots:\n")

  foreach(dir ${include_dirs})
    file(GLOB headers RELATIVE "${dir}" "${dir}/*.h")
    if(NOT headers)
      continue()
    endif()

    file(APPEND "${output_path}" "  - name: \"${dir}\"\n")
    file(APPEND "${output_path}" "    type: directory\n")
    file(APPEND "${output_path}" "    contents:\n")

    foreach(header ${headers})
      file(APPEND "${output_path}" "      - name: \"${header}\"\n")
      file(APPEND "${output_path}" "        type: file\n")
      file(APPEND "${output_path}" "        external-contents: \"${dir}/${header}\"\n")
    endforeach()
  endforeach()
endfunction()

function(generate_winsdk_lib_symlinks winsdk_um_lib_dir output_dir)
  execute_process(COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}")
  file(GLOB libraries RELATIVE "${winsdk_um_lib_dir}" "${winsdk_um_lib_dir}/*")
  foreach(library ${libraries})
    string(TOLOWER "${library}" symlink_name)
    execute_process(COMMAND "${CMAKE_COMMAND}"
                            -E create_symlink
                            "${winsdk_um_lib_dir}/${library}"
                            "${output_dir}/${symlink_name}")
  endforeach()
endfunction()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

cmake_getconf(HOST_ARCH)
cmake_getconf(MSVC_BASE)
cmake_getconf(WINSDK_BASE)
cmake_getconf(WINSDK_VER)
cmake_getconf(LLVM_VER)
cmake_getconf(CLANG_VER)
cmake_getconf(LLVM_PATH)

if(LLVM_VER STREQUAL "")
  set(LLVM_VER 11)
  message(STATUS "LLVM_VER not set assuming version ${LLVM_VER}")
endif()

if(CLANG_VER STREQUAL "")
  set(CLANG_VER 11)
  message(STATUS "CLANG_VER not set assuming version ${CLANG_VER}")
endif()

if(NOT HOST_ARCH)
  set(HOST_ARCH x86_64)
endif()
if(HOST_ARCH STREQUAL "aarch64" OR HOST_ARCH STREQUAL "arm64")
  set(TRIPLE_ARCH "aarch64")
  set(WINSDK_ARCH "arm64")
elseif(HOST_ARCH STREQUAL "armv7" OR HOST_ARCH STREQUAL "arm")
  set(TRIPLE_ARCH "armv7")
  set(WINSDK_ARCH "arm")
elseif(HOST_ARCH STREQUAL "i686" OR HOST_ARCH STREQUAL "x86")
  set(TRIPLE_ARCH "i686")
  set(WINSDK_ARCH "x86")
elseif(HOST_ARCH STREQUAL "x86_64" OR HOST_ARCH STREQUAL "x64")
  set(TRIPLE_ARCH "x86_64")
  set(WINSDK_ARCH "x64")
else()
  message(SEND_ERROR "Unknown host architecture ${HOST_ARCH}. Must be aarch64 (or arm64), armv7 (or arm), i686 (or x86), or x86_64 (or x64).")
endif()

set(MSVC_INCLUDE "${MSVC_BASE}/include")
set(MSVC_LIB "${MSVC_BASE}/lib")
set(WINSDK_INCLUDE "${WINSDK_BASE}/Include/${WINSDK_VER}")
set(WINSDK_LIB "${WINSDK_BASE}/Lib/${WINSDK_VER}")

if(NOT EXISTS "${MSVC_BASE}" OR
   NOT EXISTS "${MSVC_INCLUDE}" OR
   NOT EXISTS "${MSVC_LIB}")
  message(SEND_ERROR
          "CMake variable MSVC_BASE must point to a folder containing MSVC "
          "system headers and libraries")
endif()

if(NOT EXISTS "${WINSDK_BASE}" OR
   NOT EXISTS "${WINSDK_INCLUDE}" OR
   NOT EXISTS "${WINSDK_LIB}")
  message(SEND_ERROR
          "CMake variable WINSDK_BASE and WINSDK_VER must resolve to a valid "
          "Windows SDK installation")
endif()

if(NOT EXISTS "${WINSDK_INCLUDE}/um/Windows.h")
  message(SEND_ERROR "Cannot find Windows.h")
endif()
if(NOT EXISTS "${WINSDK_INCLUDE}/um/WINDOWS.H")
  set(case_sensitive_filesystem TRUE)
endif()

# Attempt to find the clang-cl binary
find_program(CLANG_CL_PATH NAMES clang-cl-${CLANG_VER} clang-cl PATHS ${LLVM_PATH})
if(${CLANG_CL_PATH} STREQUAL "CLANG_CL_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang-cl-${CLANG_VER}")
endif()

# Attempt to find the llvm-link binary
find_program(LLD_LINK_PATH NAMES lld-link-${LLVM_VER} lld-link PATHS ${LLVM_PATH})
if(${LLD_LINK_PATH} STREQUAL "LLD_LINK_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find lld-link-${LLVM_VER}")
endif()

# Attempt to find the lld-lib binary
find_program(LLVM_LIB_PATH NAMES llvm-lib-${LLVM_VER} llvm-lib PATHS ${LLVM_PATH})
if(${LLVM_LIB_PATH} STREQUAL "LLVM_LIB_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find llvm-lib-${LLVM_VER}")
endif()

# Attempt to find the llvm-nm binary
find_program(LLVM_NM_PATH NAMES llvm-nm-${LLVM_VER} llvm-nm PATHS ${LLVM_PATH})
if(${LLVM_NM_PATH} STREQUAL "LLVM_NM_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find llvm-nm-${LLVM_VER}")
endif()

# Attempt to find the llvm-mt binary
find_program(LLVM_MT_PATH NAMES llvm-mt-${LLVM_VER} llvm-mt PATHS ${LLVM_PATH})
#set(LLVM_MT_PATH "${CMAKE_CURRENT_LIST_DIR}/llvm-mt-wrapper")
if(${LLVM_MT_PATH} STREQUAL "LLVM_MT_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find llvm-mt-${LLVM_VER}")
endif()

# Attempt to find the native clang binary
find_program(CLANG_C_PATH NAMES clang-${CLANG_VER} clang PATHS ${LLVM_PATH})
if(${CLANG_C_PATH} STREQUAL "CLANG_C_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang-${CLANG_VER}")
endif()

# Attempt to find the native clang++ binary
find_program(CLANG_CXX_PATH NAMES clang++-${CLANG_VER} clang++ PATHS ${LLVM_PATH})
if(${CLANG_CXX_PATH} STREQUAL "CLANG_CXX_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang++-${CLANG_VER}")
endif()

# Attempt to find the llvm-rc binary
find_program(LLVM_RC_PATH NAMES llvm-rc-${LLVM_VER} llvm-rc PATHS ${LLVM_PATH})
if(${LLVM_RC_PATH} STREQUAL "LLVM_RC_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find rc")
endif()


set(CMAKE_C_COMPILER "${CLANG_CL_PATH}" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "${CLANG_CL_PATH}" CACHE FILEPATH "")
set(CMAKE_RC_COMPILER "${LLVM_RC_PATH}" CACHE FILEPATH "")
set(CMAKE_LINKER "${LLD_LINK_PATH}" CACHE FILEPATH "")
set(CMAKE_AR "${LLVM_LIB_PATH}" CACHE FILEPATH "")
set(CMAKE_NM "${LLVM_NM_PATH}" CACHE FILEPATH "")
set(CMAKE_MT "${LLVM_MT_PATH}" CACHE FILEPATH "")

# Even though we're cross-compiling, we need some native tools (e.g. llvm-tblgen), and those
# native tools have to be built before we can start doing the cross-build.  LLVM supports
# a CROSS_TOOLCHAIN_FLAGS_NATIVE argument which consists of a list of flags to pass to CMake
# when configuring the NATIVE portion of the cross-build.  By default we construct this so
# that it points to the tools in the same location as the native clang-cl that we're using.
list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_ASM_COMPILER=${CLANG_C_PATH}")
list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_C_COMPILER=${CLANG_C_PATH}")
list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_CXX_COMPILER=${CLANG_CXX_PATH}")

set(CROSS_TOOLCHAIN_FLAGS_NATIVE "${_CTF_NATIVE_DEFAULT}" CACHE STRING "")

set(COMPILE_FLAGS
    -Xclang -fexceptions -Xclang -fcxx-exceptions -Xclang
    -D_CRT_SECURE_NO_WARNINGS
    --target=${TRIPLE_ARCH}-windows-msvc
    -fms-compatibility-version=19.11
    -Wno-unused-command-line-argument # Needed to accept projects pushing both -Werror and /MP
    -imsvc "${MSVC_INCLUDE}"
    -imsvc "${WINSDK_INCLUDE}/ucrt"
    -imsvc "${WINSDK_INCLUDE}/shared"
    -imsvc "${WINSDK_INCLUDE}/um"
    -imsvc "${WINSDK_INCLUDE}/winrt")
    
link_libraries(user32 kernel32 shell32 ole32)

if(case_sensitive_filesystem)
  # Ensure all sub-configures use the top-level VFS overlay instead of generating their own.
  init_user_prop(winsdk_vfs_overlay_path)
  if(NOT winsdk_vfs_overlay_path)
    set(winsdk_vfs_overlay_path "${CMAKE_BINARY_DIR}/winsdk_vfs_overlay.yaml")
    generate_winsdk_vfs_overlay("${WINSDK_BASE}/Include/${WINSDK_VER}" "${winsdk_vfs_overlay_path}")
    init_user_prop(winsdk_vfs_overlay_path)
  endif()
  list(APPEND COMPILE_FLAGS
       -Xclang -ivfsoverlay -Xclang "${winsdk_vfs_overlay_path}")

  set(CMAKE_CLANG_VFS_OVERLAY "${winsdk_vfs_overlay_path}")
endif()

string(REPLACE ";" " " COMPILE_FLAGS "${COMPILE_FLAGS}")

# We need to preserve any flags that were passed in by the user. However, we
# can't append to CMAKE_C_FLAGS and friends directly, because toolchain files
# will be re-invoked on each reconfigure and therefore need to be idempotent.
# The assignments to the _INITIAL cache variables don't use FORCE, so they'll
# only be populated on the initial configure, and their values won't change
# afterward.
set(_CMAKE_RC_FLAGS_INITIAL -I "${MSVC_INCLUDE}"
                            -I "${WINSDK_INCLUDE}/ucrt"
                            -I "${WINSDK_INCLUDE}/shared"
                            -I "${WINSDK_INCLUDE}/um"
                            -I "${WINSDK_INCLUDE}/winrt")
string(REPLACE ";" " " _CMAKE_RC_FLAGS_INITIAL "${_CMAKE_RC_FLAGS_INITIAL}")
set(CMAKE_RC_FLAGS "${_CMAKE_RC_FLAGS_INITIAL}" CACHE STRING "" FORCE)

set(_CMAKE_C_FLAGS_INITIAL "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_C_FLAGS "${_CMAKE_C_FLAGS_INITIAL} ${COMPILE_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_CXX_FLAGS_INITIAL "${CMAKE_CXX_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${_CMAKE_CXX_FLAGS_INITIAL} ${COMPILE_FLAGS}" CACHE STRING "" FORCE)

set(LINK_FLAGS
    # Prevent CMake from attempting to invoke mt.exe. It only recognizes the slashed form and not the dashed form.
    #/manifest:no

    -libpath:"${MSVC_LIB}/${WINSDK_ARCH}"
    -libpath:"${WINSDK_LIB}/ucrt/${WINSDK_ARCH}"
    -libpath:"${WINSDK_LIB}/um/${WINSDK_ARCH}")

if(case_sensitive_filesystem)
  # Ensure all sub-configures use the top-level symlinks dir instead of generating their own.
  init_user_prop(winsdk_lib_symlinks_dir)
  if(NOT winsdk_lib_symlinks_dir)
    set(winsdk_lib_symlinks_dir "${CMAKE_BINARY_DIR}/winsdk_lib_symlinks")
    generate_winsdk_lib_symlinks("${WINSDK_BASE}/Lib/${WINSDK_VER}/um/${WINSDK_ARCH}" "${winsdk_lib_symlinks_dir}")
    init_user_prop(winsdk_lib_symlinks_dir)
  endif()
  list(APPEND LINK_FLAGS
       -libpath:"${winsdk_lib_symlinks_dir}")
endif()

string(REPLACE ";" " " LINK_FLAGS "${LINK_FLAGS}")

# See explanation for compiler flags above for the _INITIAL variables.
set(_CMAKE_EXE_LINKER_FLAGS_INITIAL "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${_CMAKE_EXE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_MODULE_LINKER_FLAGS_INITIAL "${CMAKE_MODULE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_MODULE_LINKER_FLAGS "${_CMAKE_MODULE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_SHARED_LINKER_FLAGS_INITIAL "${CMAKE_SHARED_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_SHARED_LINKER_FLAGS "${_CMAKE_SHARED_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

# CMake populates these with a bunch of unnecessary libraries, which requires
# extra case-correcting symlinks and what not. Instead, let projects explicitly
# control which libraries they require.
set(CMAKE_C_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)

if(NOT $ENV{VCPKG_TOOLCHAIN} STREQUAL "")
  message(STATUS "Included VCPKG: $ENV{VCPKG_TOOLCHAIN}")
  include($ENV{VCPKG_TOOLCHAIN})
endif()