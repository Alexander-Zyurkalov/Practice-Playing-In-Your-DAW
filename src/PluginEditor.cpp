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
    trackListBox.setModel(&trackListBoxModel);
    trackListBox.setBounds(300, 300, 600,600);
    trackListBox.setOpaque(false);
//    trackListBox.setSize(200, 200);
    addAndMakeVisible(trackListBox);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    TrackListSingleton::deleteInstance();
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
    using Px = juce::Grid::Px;

    juce::Grid grid;

    grid.templateRows    = {
            Track(Px(30)),
            Track(Fr(1)) };
    grid.templateColumns = {
            Track(Fr(1)),
    };

    grid.items = {
            juce::GridItem(timeSigPanel).withMargin(juce::GridItem::Margin{10, 0,0,0}),
            juce::GridItem(viewport).withMargin(juce::GridItem::Margin{10, 0, 0 ,0}),
    };

    grid.performLayout(getLocalBounds());
}


void AudioPluginAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source) {
    const TimeSignature &sig = processorRef.getTimeSignature();
    noteGrid.updateTimeSignature(sig);
    timeSigPanel.newTimeSignature(sig.getNumerator(), sig.getDenominator());
}