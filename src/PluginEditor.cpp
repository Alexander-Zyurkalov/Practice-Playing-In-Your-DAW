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
    addAndMakeVisible(timeSigPanel);
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
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    juce::Grid grid;

    grid.templateRows    = { Track(juce::Grid::Px(50)), Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)) };

    grid.items = { juce::GridItem(timeSigPanel), juce::GridItem(noteGrid) };

    grid.performLayout(getLocalBounds());
}


void AudioPluginAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source) {
    noteGrid.timeSignature = processorRef.getTimeSignature();
    noteGrid.repaint();
}
