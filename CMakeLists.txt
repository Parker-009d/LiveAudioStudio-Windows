cmake_minimum_required(VERSION 3.22)

project(LiveAudioStudio VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(../JUCE JUCE)

if(APPLE)
    juce_add_gui_app(LiveAudioStudio
        PRODUCT_NAME "Live Audio Studio"
        COMPANY_NAME "LiveAudioStudio"
        ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/LiveAudioStudioIcon.png"
        MICROPHONE_PERMISSION_ENABLED TRUE
        MICROPHONE_PERMISSION_TEXT "Live Audio Studio needs microphone access to receive audio from your audio interface."
    )
else()
    juce_add_gui_app(LiveAudioStudio
        PRODUCT_NAME "Live Audio Studio"
        COMPANY_NAME "LiveAudioStudio"
    )
endif()

juce_generate_juce_header(LiveAudioStudio)

target_sources(LiveAudioStudio PRIVATE
    Source/Main.cpp
    Source/MainComponent.h
    Source/MainComponent.cpp
)

target_compile_features(LiveAudioStudio PRIVATE cxx_std_17)

target_compile_definitions(LiveAudioStudio PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

target_link_libraries(LiveAudioStudio PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
)
