set(LIB_SOURCES
    beat_detector.cpp
    chord_detector.cpp
    chord_tpl_collection.cpp
    chord_tpl.cpp
    cqt_wrapper.cpp
    envelope.cpp
    fft.cpp
    fft_wrapper.cpp
    lmhelpers.cpp
    lmlogger.cpp
    lmpriority_queue.cpp
    lmtypes.cpp
    ma_filter.cpp
    music_scale.cpp
    pitch_calculator.cpp
    pcp_buf.cpp
    pitch_cls_profile.cpp
    recursive_filter.cpp
    tft.cpp
    transform.cpp
    viterbi.cpp
    window_functions.cpp
)

add_library(music-dsp SHARED ${LIB_SOURCES})

add_dependencies(${MUSIC_DSP_TARGET} ${EXT_CQTT_TARGET})

target_link_libraries(${MUSIC_DSP_TARGET} ${EXT_CQTT_TARGET})

