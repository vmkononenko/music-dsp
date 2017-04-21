cmake_minimum_required (VERSION 3.1)

project (music-dsp CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../build/lib)

add_subdirectory (src)