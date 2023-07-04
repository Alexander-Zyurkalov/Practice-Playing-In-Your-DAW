//
// Created by Alexandr Zyurkalov on 04.07.23.
//
#ifndef PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H
#define PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H

#include <juce_gui_basics/juce_gui_basics.h>

class TimeSignaturePanel : public juce::Component
{
public:
    TimeSignaturePanel();

    void resized() override;

    void timeSignatureChanged();

    void numBarsChanged();

private:
    juce::Label timeSigLabel;
    juce::Label numBarsLabel;

    juce::Slider timeSigNumeratorBox;
    juce::ComboBox timeSigDenominatorBox;
    juce::Slider numBarsBox;
};

#endif //PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H

