set(LIB_SOURCES
    cheby1_filter.cpp
    chord_detector.cpp
    chord_tpl_collection.cpp
    chord_tpl.cpp
    envelope.cpp
    fft.cpp
    lmhelpers.cpp
    lmlogger.cpp
    lmpriority_queue.cpp
    lmtypes.cpp
    ma_filter.cpp
    music_scale.cpp
    pitch_calculator.cpp
    pitch_cls_profile.cpp
    window_functions.cpp
)

add_library(music-dsp SHARED ${LIB_SOURCES})
