#pragma once

#include "PluginProcessor.h"
#include "gui/NoteGrid.h"
#include "gui/TimeSignaturePanel.h"
#include "juce_audio_utils/juce_audio_utils.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::ChangeListener
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
    juce::Viewport viewport;
    juce::ListBox trackListBox;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
