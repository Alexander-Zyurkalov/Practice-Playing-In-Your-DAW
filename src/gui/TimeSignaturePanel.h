//
// Created by Alexandr Zyurkalov on 04.07.23.
//
#ifndef PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H
#define PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H

#include <juce_gui_basics/juce_gui_basics.h>
#include "NoteGrid.h"
#include "RecordButtonLookAndFeel.h"

class TimeSignaturePanel : public juce::Component, public juce::Button::Listener
{
public:
    TimeSignaturePanel(NoteGrid& nGrid, AudioPluginAudioProcessor* audioProcessor);
    TimeSignaturePanel() = delete;

    void resized() override;

    void timeSignatureChanged();

    void numBarsChanged();

    void newTimeSignature(int num, int denom);

    void newNumBars(int numBars);

    void buttonClicked(juce::Button *button) override;

private:
    juce::Label timeSigLabel;
    juce::Label numBarsLabel;

    juce::Slider timeSigNumeratorBox;
    juce::ComboBox timeSigDenominatorBox;
    RecordButtonLookAndFeel recordButtonLookAndFeel;
    juce::Slider numBarsBox;
    NoteGrid& noteGrid;
    juce::TextButton recordButton;
    AudioPluginAudioProcessor* audioPluginAudioProcessor;

};

#endif //PRACTICEPLAYINGINDAW_TIMESIGNATUREPANEL_H

