file (GLOB TOOLS_SUBDIR_C tools/*.c)
list(REMOVE_ITEM TOOLS_SUBDIR_C "${CMAKE_CURRENT_SOURCE_DIR}/tools/psdpng.c")

message (${TOOLS_SUBDIR_C})

set(LIB_SOURCES
    ${TOOLS_SUBDIR_C}
    kiss_fft.c
)

add_library(${EXT_KISSFFT_TARGET} STATIC ${LIB_SOURCES})

target_compile_definitions(${EXT_KISSFFT_TARGET} INTERFACE kiss_fft_scalar=double)