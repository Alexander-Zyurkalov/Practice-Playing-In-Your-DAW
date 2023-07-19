//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "NoteBars.h"
#include <juce_audio_basics/juce_audio_basics.h>

NoteBars::NoteBars()
{
    setOpaque(false);
}

void NoteBars::paint(juce::Graphics& g)
{
    int pixelsPerQuarterNote = 100; //TODO: it is the second place where we use this constant. Do something with that, DRY!!!
    double ppqLeftPosition = dawTransportData.getPpqStartLoopPosition();
    double ppqRightPosition = dawTransportData.getPpqEndLoopPosition() + (double)getHeight()/pixelsPerQuarterNote;
    for (auto& note : notes)
    {
        if (ppqLeftPosition < note.getPpqStartPosition() && ppqRightPosition < note.getPpqStartPosition())
            continue;
        if (ppqLeftPosition> note.getPpqReleasePosition() && ppqRightPosition > note.getPpqReleasePosition())
            continue;

        const juce::Rectangle<int>& rectangle = getNoteRectangle(note);

        g.setColour( juce::Colours::lightskyblue);
        g.fillRect(rectangle);
        g.setColour(juce::Colours::black);
        g.drawRect(rectangle, 1);

        const juce::Rectangle<int> &widthOfText = rectangle.withWidth(rectangle.getHeight()+10);

        g.setColour(juce::Colours::black);
        g.drawText(note.getNoteName(), widthOfText, juce::Justification::left);
    }
}


void NoteBars::resized()
{
}



juce::Rectangle<int> NoteBars::getNoteRectangle(const MPENoteEvent &note)
{
    int pixelsPerQuarterNote = 100;
    double ppqStartPosition = note.getPpqStartPosition() - dawTransportData.getPpqStartLoopPosition();
    double ppqReleasePosition =  note.getPpqReleasePosition() - dawTransportData.getPpqStartLoopPosition();
    double noteLength = ppqReleasePosition - ppqStartPosition;
    if (noteLength <= 0.0)
        noteLength = 1.0/8;
    auto noteX = static_cast<float>(ppqStartPosition * pixelsPerQuarterNote);
    const int numPitches = 128;
    float noteHeight = (float)getHeight() / numPitches;
    float noteY = static_cast<float>(numPitches - note.getMpeNote().initialNote - 1.0) * noteHeight;
    auto noteWidth = static_cast<float>(noteLength * pixelsPerQuarterNote);
    return juce::Rectangle<int>{static_cast<int>(noteX), static_cast<int>(noteY), static_cast<int>(noteWidth), static_cast<int>(noteHeight)};
}
