set(LIB_SOURCES
    Chromagram.cpp
    ConstantQ.cpp
    CQInverse.cpp
    CQKernel.cpp
    CQSpectrogram.cpp
    Pitch.cpp
)

add_subdirectory(dsp)

add_library(${EXT_CQTT_TARGET} STATIC ${LIB_SOURCES})
