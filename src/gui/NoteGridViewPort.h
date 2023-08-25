//
// Created by Alexandr Zyurkalov on 24.08.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
#define PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
#include <juce_gui_basics/juce_gui_basics.h>
class Lag
{
public:
    Lag(double initial = -1.0f, double rate = 0.04f);
    void setTarget(double newTarget);
    void update();
    double getValue() const;
    void setRate(double newRate);
    void setValue(double newValue);
private:
    double value;  // Current value
    double target; // Target value
    double rate;   // Rate of change towards the target (0 to 1)
};

class NoteGridViewPort: public juce::Viewport, private juce::Timer
{
public:
    NoteGridViewPort();

    void scrollBarMoved(juce::ScrollBar *bar, double newRangeStart) override;

    void setViewPositionProportionately (const double x, const double y);

    bool isManuallyScrolled() const
    {
        return manuallyScrolled;
    }

private:
    void timerCallback() override;
    bool manuallyScrolled;
    Lag xLag;
    Lag yLag;

};


#endif //PRACTICEPLAYINGINDAW_NOTEGRIDVIEWPORT_H
