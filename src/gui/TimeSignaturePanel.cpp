//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#include "TimeSignaturePanel.h"

TimeSignaturePanel::TimeSignaturePanel()
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
    timeSigDenominatorBox.onChange = [this] { timeSignatureChanged(); };

    addAndMakeVisible(numBarsBox);
    numBarsBox.setRange(1, 64, 1);
    numBarsBox.setValue(4);
    numBarsBox.onValueChange = [this] { numBarsChanged(); };

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
            juce::Grid::TrackInfo(juce::Grid::Fr(2)) };

    grid.items = {
            juce::GridItem(timeSigNumeratorBox).withMargin(juce::GridItem::Margin(0,10,0,0)),
            juce::GridItem(timeSigDenominatorBox).withMargin(juce::GridItem::Margin(0,10,0,10)),
            juce::GridItem(numBarsBox).withMargin(juce::GridItem::Margin(0,10,0,10)) };
    grid.performLayout(getLocalBounds());
}

void TimeSignaturePanel::timeSignatureChanged()
{
    // Do something when the time signature is changed
}

void TimeSignaturePanel::numBarsChanged()
{
    // Do something when the number of bars is changed
}
