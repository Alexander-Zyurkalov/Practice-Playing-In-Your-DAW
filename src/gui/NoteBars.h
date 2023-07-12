//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEBARS_H
#define PRACTICEPLAYINGINDAW_NOTEBARS_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "../processing/MPENoteEvent.h"
#include "../processing/DAWTransportData.h"

class NoteBars: public juce::Component
{
public:
    NoteBars();
    ~NoteBars() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateDAWTransportData(DAWTransportData transportData)
    {
        dawTransportData = transportData;
        repaint();
    }

private:
    std::vector<MPENoteEvent> notes;
    DAWTransportData dawTransportData;

    juce::Rectangle<int> getNoteRectangle(const MPENoteEvent& note);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteBars)
};


#endif //PRACTICEPLAYINGINDAW_NOTEBARS_H
