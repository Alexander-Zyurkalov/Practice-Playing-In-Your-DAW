//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_NOTEGRID_H
#define PRACTICEPLAYINGINDAW_NOTEGRID_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "../PluginProcessor.h"
#include "../processing/DAWTransportData.h"
#include "NoteBars.h"
#include "NoteGridViewPort.h"
#include <memory>

class NoteGrid : public juce::Component
{
public:
    NoteGrid(NoteGridViewPort& viewport);
    ~NoteGrid() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateDAWTransportData(DAWTransportData transportData);

    NoteBars noteBars;
private:
    NoteGridViewPort* viewport;
    DAWTransportData dawTransportData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteGrid)

    void drawTheCell(int beatPosition, bool isBar, juce::Graphics &g, juce::Rectangle<float> &visibleArea,
                     juce::Rectangle<int> bound);
};


#endif //PRACTICEPLAYINGINDAW_NOTEGRID_H
