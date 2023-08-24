#pragma once

#include "PluginProcessor.h"
#include "gui/NoteGrid.h"
#include "gui/TimeSignaturePanel.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "gui/NoteGridViewPort.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener,
        private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    AudioPluginAudioProcessor& processorRef;
    NoteGrid noteGrid;
    TimeSignaturePanel timeSigPanel;
    NoteGridViewPort viewport;
    juce::ListBox trackListBox;
    void timerCallback() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
