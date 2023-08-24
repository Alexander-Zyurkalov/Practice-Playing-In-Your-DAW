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
