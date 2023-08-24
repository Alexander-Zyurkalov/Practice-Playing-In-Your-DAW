//
// Created by Alexandr Zyurkalov on 24.08.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
#define PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
#include <juce_gui_basics/juce_gui_basics.h>

class NoteGridViewPort: public juce::Viewport, private juce::Timer
{
public:
    NoteGridViewPort();

    void scrollBarMoved(juce::ScrollBar *bar, double newRangeStart) override;

    bool isManuallyScrolled() const
    {
        return manuallyScrolled;
    }

private:
    void timerCallback() override;
    bool manuallyScrolled;
};


#endif //PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
