set(LIB_SOURCES
    chord_detector.cpp
    chord_tpl_collection.cpp
    chord_tpl.cpp
    fft.cpp
    lmhelpers.cpp
    lmlogger.cpp
    lmpriority_queue.cpp
    lmtypes.cpp
    music_scale.cpp
    pitch_calculator.cpp
    pitch_cls_profile.cpp
    window_functions.cpp
)

add_library(music-dsp SHARED ${LIB_SOURCES})
