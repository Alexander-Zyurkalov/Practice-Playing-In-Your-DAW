#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/NoteGrid.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), noteGrid(viewport), timeSigPanel(noteGrid, &p)
{
    juce::ignoreUnused (processorRef);
    setSize (processorRef.getWindowWidth(), processorRef.getWindowHeight());
    setResizable(true, true);
    processorRef.addChangeListener(this);
    viewport.setViewedComponent(&noteGrid, false);
    addAndMakeVisible(viewport);
    addAndMakeVisible(timeSigPanel);
    startTimer(500);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    stopTimer();
    processorRef.removeChangeListener(this);
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
            Track(Fr(1)) ,
    };
    grid.templateColumns = {
            Track(Fr(1)),
    };

    grid.items = {
            juce::GridItem(timeSigPanel).withMargin(juce::GridItem::Margin{10, 0,0,0}),
            juce::GridItem(viewport).withMargin(juce::GridItem::Margin{10, 0, 0 ,0}),
    };
    trackListBox.updateContent();

    grid.performLayout(getLocalBounds());
    changeListenerCallback(nullptr);
    processorRef.setWindowWidth(getWidth());
    processorRef.setWindowHeight(getHeight());
}


void AudioPluginAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source) {
    const DAWTransportData &dawTransportData = processorRef.getDAWTransportData();
    noteGrid.noteBars.notes = processorRef.getNoteEventVector();
    noteGrid.updateDAWTransportData(dawTransportData);
    timeSigPanel.newTimeSignature(dawTransportData.getNumerator(), dawTransportData.getDenominator());
    timeSigPanel.newNumBars(dawTransportData.getNumBars());
    timeSigPanel.updateButtonState();
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    setSize(processorRef.getWindowWidth(), processorRef.getWindowHeight());
}
