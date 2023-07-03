cmake_minimum_required(VERSION 3.25)
project(PracticePlayingInDAW)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./libs/juce)

juce_add_plugin(PracticePlayingInDAW
        COMPANY_NAME yourcompany
        VERSION 1.0.0
        PLUGIN_MANUFACTURER_CODE Manu
        PLUGIN_CODE Plg1
        FORMATS VST3 Standalone   # This line enables VST3 and Standalone formats
        ProductName "Your Plugin Name")

target_sources(PracticePlayingInDAW PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
        src/gui/NoteGrid.cpp
)

target_compile_definitions(PracticePlayingInDAW
        PRIVATE JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(PracticePlayingInDAW PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_basics
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        )
