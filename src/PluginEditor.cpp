#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/NoteGrid.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), timeSigPanel(noteGrid)
{
    juce::ignoreUnused (processorRef);
    setSize (1200, 600);
    setResizable(true, true);
    processorRef.addChangeListener(this);
    viewport.setViewedComponent(&noteGrid, false);
    addAndMakeVisible(viewport);
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

    grid.templateRows    = { Track(juce::Grid::Px(20)), Track(Fr(1)) };
    grid.templateColumns = { Track(Fr(1)) };

    grid.items = { juce::GridItem(timeSigPanel), juce::GridItem(viewport) };

    grid.performLayout(getLocalBounds());
}


void AudioPluginAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source) {
    noteGrid.updateTimeSignature(processorRef.getTimeSignature());
}