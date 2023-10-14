//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEBARS_H
#define PRACTICEPLAYINGINDAW_NOTEBARS_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "../processing/MPENoteEvent.h"
#include "../processing/DAWTransportData.h"
#include "NoteGridViewPort.h"

class NoteBars: public juce::Component
{
public:
    NoteBars(NoteGridViewPort& viewport);
    ~NoteBars() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateDAWTransportData(DAWTransportData transportData)
    {
        dawTransportData = transportData;
        repaint();
    }

    std::vector<MPENoteEvent> notes;

    float getMiddleYPosition() const;
private:
    NoteGridViewPort* viewport;
    DAWTransportData dawTransportData;
    float middleYPosition = 0.0f;

    juce::Rectangle<int> getNoteRectangle(const MPENoteEvent& note);

    void
    drawVelocityLine(const juce::Graphics &g, const MPENoteEvent &note, const juce::Rectangle<int> &rectangle) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteBars)

    void setColourByShift(juce::Graphics &g, float shift) const;
};


#endif //PRACTICEPLAYINGINDAW_NOTEBARS_H
