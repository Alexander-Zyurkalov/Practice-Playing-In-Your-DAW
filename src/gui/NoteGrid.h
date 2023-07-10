//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEGRID_H
#define PRACTICEPLAYINGINDAW_NOTEGRID_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "../PluginProcessor.h"
#include "../processing/TimeSignature.h"
#include "NoteBars.h"
#include <memory>

class NoteGrid : public juce::Component
{
public:
    NoteGrid(juce::Viewport& viewport);
    ~NoteGrid() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateTimeSignature(TimeSignature timeSig)
    {
        this->timeSignature = timeSig;
        repaint();
    }
private:
    juce::Viewport* viewport;
    TimeSignature timeSignature;
    NoteBars noteBars;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteGrid)
};


#endif //PRACTICEPLAYINGINDAW_NOTEGRID_H
