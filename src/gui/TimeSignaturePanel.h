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
        addAndMakeVisible(timeSigLabel);
        timeSigLabel.setText("Time Signature:", juce::dontSendNotification);
        timeSigLabel.attachToComponent(&timeSigBox, true);

        addAndMakeVisible(timeSigBox);
        timeSigBox.addItem("2/4", 1);
        timeSigBox.addItem("3/4", 2);
        timeSigBox.addItem("4/4", 3);
        timeSigBox.onChange = [this] { timeSignatureChanged(); };

        addAndMakeVisible(numBarsLabel);
        numBarsLabel.setText("Number of Bars:", juce::dontSendNotification);
        numBarsLabel.attachToComponent(&numBarsBox, true);

        addAndMakeVisible(numBarsBox);
        numBarsBox.setSliderStyle(juce::Slider::IncDecButtons);
        numBarsBox.setRange(1, 64, 1);
        numBarsBox.setValue(4);
        numBarsBox.onValueChange = [this] { numBarsChanged(); };
    }

    void resized() override
    {
        auto area = getLocalBounds();
        timeSigBox.setBounds(area.removeFromTop(25));
        numBarsBox.setBounds(area.removeFromTop(25));


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
    juce::ComboBox timeSigBox;

    juce::Label numBarsLabel;
    juce::Slider numBarsBox;
};


#endif //PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H
