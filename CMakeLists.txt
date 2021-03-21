cmake_minimum_required (VERSION 3.1)

project (MusicDsp C CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if (NOT CMAKE_LIBRARY_PATH)
    set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

set (LMCLIENT_TARGET lmclient)
set (MUSIC_DSP_TARGET music-dsp)
set (TESTS_TARGET tests)
set (VAMP_TARGET parachord-vamp)
set (EXT_CQTT_TARGET cqtt)
set (EXT_KISSFFT_TARGET kissfft)
set (EXT_VAMPSDK_TARGET vamp-plugin-sdk)

include_directories(
    ${PROJECT_SOURCE_DIR}/libmusic/include
    ${PROJECT_SOURCE_DIR}/ext/cqtt/include
    ${PROJECT_SOURCE_DIR}/ext/kissfft
    ${PROJECT_SOURCE_DIR}/ext/kissfft/tools
)

find_path(SND_HEADERS NAMES sndfile.h)
find_library(LIBSNDFILE NAMES sndfile)

if (DEFINED ANDROID_ABI)
    if (${ANDROID_ABI} STREQUAL "x86_64")
        include_directories(${ANDROID_SYSROOT}/usr/include/x86_64-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "x86")
        include_directories(${ANDROID_SYSROOT}/usr/include/i686-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
        include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
        include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
    endif()
endif()

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

option(WITH_CLIENT "Build console client" OFF)
option(WITH_TESTS "Build unit tests" OFF)
option(WITH_VAMP "Build Parachord VAMP plugin" ON)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(ext)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")

if(WITH_CLIENT)
    add_subdirectory(client)
endif()

if(WITH_VAMP)
    add_subdirectory(vamp)
endif()

add_subdirectory(libmusic)
