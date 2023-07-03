#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/NoteGrid.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (400, 300);
    setResizable(true, true);
    processorRef.addChangeListener(this);
    addAndMakeVisible(noteGrid);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    noteGrid.setBounds (getLocalBounds());
}

void AudioPluginAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source) {
    noteGrid.timeSignature = processorRef.getTimeSignature();
    repaint();
}
