//
// Created by Alexandr Zyurkalov on 24.08.23.
//

#include "NoteGridViewPort.h"


void NoteGridViewPort::scrollBarMoved(juce::ScrollBar *bar, double newRangeStart)
{
    Viewport::scrollBarMoved(bar, newRangeStart);
    manuallyScrolled = true;
    startTimer(1000);
}

void NoteGridViewPort::timerCallback()
{
    manuallyScrolled = false;
    stopTimer();
}

NoteGridViewPort::NoteGridViewPort(): manuallyScrolled(false)
{

}

void NoteGridViewPort::setViewPositionProportionately(const double x, const double y)
{
    if (xLag.getValue() < 0.0f)
        xLag.setValue(x);
    if (yLag.getValue() < 0.0f)
        yLag.setValue(y);

    if (manuallyScrolled)
    {
        xLag.setValue(getViewPositionX()/4500.0);
        yLag.setValue(getViewPositionY()/3000.0);
    }
    else
    {
        xLag.setTarget(x);
        yLag.setTarget(y);
        xLag.update();
        double xlaged = xLag.getValue();
        double ylaged = yLag.getValue();
        Viewport::setViewPositionProportionately(x, y);
        manuallyScrolled = false;
    }

}


Lag::Lag(double initial, double rate)
        : value(initial), target(initial), rate(rate)
{
}

void Lag::setTarget(double newTarget)
{
    target = newTarget;
}

void Lag::update()
{
    value += (target - value) * rate;
}

double Lag::getValue() const
{
    return value;
}

void Lag::setRate(double newRate)
{
    rate = juce::jlimit(0.0, 1.0, newRate);
}

void Lag::setValue(double newValue)
{
    value = newValue;
    target = newValue;
}
