//
// Created by Alexandr Zyurkalov on 04.07.23.
//
#ifndef PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H
#define PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H

#include <juce_gui_basics/juce_gui_basics.h>

class TimeSignaturePanel : public juce::Component
{
public:
    TimeSignaturePanel()
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
    }

    void resized() override
    {
        juce::Grid grid;

        grid.templateRows    = { juce::Grid::TrackInfo(juce::Grid::Fr(1)) };
        grid.templateColumns = {
                juce::Grid::TrackInfo(juce::Grid::Fr(1)),
                juce::Grid::TrackInfo(juce::Grid::Px(200)),
                juce::Grid::TrackInfo(juce::Grid::Fr(1))
        };

        grid.items = { juce::GridItem(timeSigNumeratorBox), juce::GridItem(timeSigDenominatorBox), juce::GridItem(numBarsBox) };
        grid.performLayout(getLocalBounds());
    }

    void timeSignatureChanged()
    {
        // Do something when the time signature is changed
    }

    void numBarsChanged()
    {
        // Do something when the number of bars is changed
    }

private:
    juce::Label timeSigLabel;
    juce::Label numBarsLabel;

    juce::Slider timeSigNumeratorBox;
    juce::ComboBox timeSigDenominatorBox;
    juce::Slider numBarsBox;
};

#endif //PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H
