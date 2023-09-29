cmake_minimum_required(VERSION 3.25)
project(PracticePlayingInDAW)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(./libs/juce)

juce_add_plugin(PracticePlayingInDAW
        COMPANY_NAME Zyurkalov
        VERSION 0.0.1
        PLUGIN_MANUFACTURER_CODE Zyur
        PLUGIN_CODE PrPl
        FORMATS VST3 Standalone   # This line enables VST3 and Standalone formats
        ProductName "Practice Playing In DAW"
        MIDI_INPUTS "1"
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT TRUE
        MIDI_EFFECT_ONLY FALSE
)

target_sources(PracticePlayingInDAW PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
        src/gui/NoteGrid.cpp
        src/gui/TimeSignaturePanel.cpp
        src/gui/NoteBars.cpp
        src/gui/RecordButtonLookAndFeel.cpp
        src/gui/NoteGridViewPort.cpp
        src/processing/MyMPEInstrumentListener.cpp
        src/processing/MPENoteEvent.cpp
        src/processing/DAWTransportData.cpp

)

target_compile_definitions(PracticePlayingInDAW
        PRIVATE JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(PracticePlayingInDAW
        PRIVATE
            juce::juce_audio_utils
            juce::juce_dsp
            juce::juce_gui_basics
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags
        )


add_subdirectory(./libs/catch2)
add_executable(test
        test/MyMPEInstrumentListenerTest.cpp
        src/processing/MyMPEInstrumentListener.cpp
        src/processing/MPENoteEvent.cpp
        src/processing/DAWTransportData.cpp
        test/DAWTransportData.cpp
)
target_compile_definitions(test
        PRIVATE
                JUCE_WEB_BROWSER=0
                JUCE_USE_CURL=0
                JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(test
        PRIVATE
                Catch2::Catch2WithMain
                juce::juce_audio_utils
                juce::juce_dsp
                juce::juce_gui_basics
        PUBLIC
                juce::juce_recommended_config_flags
                juce::juce_recommended_lto_flags
                juce::juce_recommended_warning_flags
)
