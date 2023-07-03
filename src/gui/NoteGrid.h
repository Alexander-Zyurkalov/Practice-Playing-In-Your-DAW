//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEGRID_H
#define PRACTICEPLAYINGINDAW_NOTEGRID_H

#include "juce_gui_basics/juce_gui_basics.h"
class NoteGrid : public juce::Component
{
public:
    NoteGrid();
    ~NoteGrid() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteGrid)
};


#endif //PRACTICEPLAYINGINDAW_NOTEGRID_H
