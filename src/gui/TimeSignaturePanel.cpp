//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#include "TimeSignaturePanel.h"
#include "RecordButtonLookAndFeel.h"

TimeSignaturePanel::TimeSignaturePanel(NoteGrid& nGrid, AudioPluginAudioProcessor* audioProcessor) :
        noteGrid{nGrid}, audioPluginAudioProcessor{audioProcessor}
{
    addAndMakeVisible(timeSigNumeratorBox);
    timeSigNumeratorBox.setRange(1, 64, 1);
    timeSigNumeratorBox.setValue(4);
    timeSigNumeratorBox.onValueChange = [this] { timeSignatureChanged(); };


    addAndMakeVisible(timeSigDenominatorBox);
    timeSigDenominatorBox.addItem("1", 1);
    timeSigDenominatorBox.addItem("2", 2);
    timeSigDenominatorBox.addItem("4", 3);
    timeSigDenominatorBox.addItem("8", 4);
    timeSigDenominatorBox.addItem("16", 5);
    timeSigDenominatorBox.addItem("32", 6);
    timeSigDenominatorBox.addItem("64", 7);
    timeSigDenominatorBox.setSelectedId(3);
    timeSigDenominatorBox.onChange = [this] { timeSignatureChanged(); };

    addAndMakeVisible(numBarsBox);
    numBarsBox.setRange(1, 64, 1);
    numBarsBox.setValue(16);
    numBarsBox.onValueChange = [this] { timeSignatureChanged(); };


    recordButton.setLookAndFeel(&recordButtonLookAndFeel);
    recordButton.setClickingTogglesState(true);
    recordButton.addListener(this);
    recordButton.setToggleState(audioPluginAudioProcessor->isRecording(),
                                juce::NotificationType::dontSendNotification);
    addAndMakeVisible(recordButton);

    if (!juce::JUCEApplicationBase::isStandaloneApp())
    {
        timeSigNumeratorBox.setEnabled(false);
        timeSigDenominatorBox.setEnabled(false);
        numBarsBox.setEnabled(false);
    }
}

void TimeSignaturePanel::resized()
{
    juce::Grid grid;

    grid.templateRows    = { juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
    grid.templateColumns = {
            juce::Grid::TrackInfo(juce::Grid::Fr(1)),
            juce::Grid::TrackInfo(juce::Grid::Px(100)),
            juce::Grid::TrackInfo(juce::Grid::Fr(1)),
            juce::Grid::TrackInfo(juce::Grid::Px(50)),
    };

    grid.items = {
            juce::GridItem(timeSigNumeratorBox).withMargin(juce::GridItem::Margin(0,10,0,0)),
            juce::GridItem(timeSigDenominatorBox).withMargin(juce::GridItem::Margin(0,10,0,10)),
            juce::GridItem(numBarsBox).withMargin(juce::GridItem::Margin(0,10,0,10)),
            juce::GridItem(&recordButton).withMargin(juce::GridItem::Margin(0,10,0,10))
    };
    grid.performLayout(getLocalBounds());
}

void TimeSignaturePanel::timeSignatureChanged()
{
    int denominator;
    switch (timeSigDenominatorBox.getSelectedId()) {
        case 1: denominator = 1; break;
        case 2: denominator = 2; break;
        case 3: denominator = 4; break;
        case 4: denominator = 8; break;
        case 5: denominator = 16; break;
        case 6: denominator = 32; break;
        case 7: denominator = 64; break;
        default: denominator = 4; break;
    }
    DAWTransportData newTimeSignature{
            static_cast<int>(timeSigNumeratorBox.getValue()),
            denominator};
    double endLoopPos = numBarsBox.getValue() * newTimeSignature.getNumerator() * 4/newTimeSignature.getDenominator();
    newTimeSignature.set(0, 0, endLoopPos);
    noteGrid.updateDAWTransportData(newTimeSignature);
}

void TimeSignaturePanel::numBarsChanged()
{
    // Do something when the number of bars is changed
}

void TimeSignaturePanel::newTimeSignature(int num, int denom) {
    timeSigNumeratorBox.setValue(num, juce::dontSendNotification);
    int selectedId;
    switch (denom) {
        case 1: selectedId = 1; break;
        case 2: selectedId = 2; break;
        case 4: selectedId = 3; break;
        case 8: selectedId = 4; break;
        case 16: selectedId = 5; break;
        case 32: selectedId = 6; break;
        case 64: selectedId = 7; break;
        default: selectedId = 3; break;
    }
    timeSigDenominatorBox.setSelectedId(selectedId, juce::dontSendNotification);
}

void TimeSignaturePanel::newNumBars(int numBars) {
    numBarsBox.setValue(numBars, juce::dontSendNotification);
}

void TimeSignaturePanel::buttonClicked(juce::Button *button) {
    audioPluginAudioProcessor->toggleRecording();
}

void TimeSignaturePanel::updateButtonState() {
    recordButton.setToggleState(audioPluginAudioProcessor->isRecording(),
                                juce::NotificationType::dontSendNotification);
}
