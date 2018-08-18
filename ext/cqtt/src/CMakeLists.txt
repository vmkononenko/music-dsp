file (GLOB DSP_SUBDIR_CPP dsp/*.cpp)

set(LIB_SOURCES
    ${DSP_SUBDIR_CPP}
    Chromagram.cpp
    ConstantQ.cpp
    CQInverse.cpp
    CQKernel.cpp
    CQSpectrogram.cpp
    Pitch.cpp
)

add_subdirectory(dsp)

add_library(${EXT_CQTT_TARGET} STATIC ${LIB_SOURCES})

add_dependencies(${EXT_CQTT_TARGET} ${EXT_KISSFFT_TARGET})

target_link_libraries(${EXT_CQTT_TARGET} ${EXT_KISSFFT_TARGET})