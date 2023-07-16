//
// Created by Alexandr Zyurkalov on 16.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_RECORDBUTTONLOOKANDFEEL_H
#define PRACTICEPLAYINGINDAW_RECORDBUTTONLOOKANDFEEL_H

#include "juce_gui_basics/juce_gui_basics.h"
class RecordButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour,
                                                   shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        
        juce::Path recordIcon;
        const float iconSize = juce::jmin(button.getWidth(), button.getHeight()) * 0.7f;
        recordIcon.addEllipse(button.getWidth() * 0.5f - iconSize * 0.5f,
                              button.getHeight() * 0.5f - iconSize * 0.5f,
                              iconSize, iconSize);

        juce::Colour colour = button.getToggleState() ? juce::Colours::red : juce::Colours::grey;
        g.setColour(colour);
        g.fillPath(recordIcon);
    }
};



#endif //PRACTICEPLAYINGINDAW_RECORDBUTTONLOOKANDFEEL_H
