//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "NoteBars.h"
#include <juce_audio_basics/juce_audio_basics.h>

NoteBars::NoteBars()
{
    setOpaque(false);
    for (int i=0; i<128; ++i)
    {
        const juce::MPENote &note = juce::MPENote(1,  i, juce::MPEValue(), juce::MPEValue(),
                                                  juce::MPEValue(), juce::MPEValue(),juce::MPENote::KeyState::keyDown);
        MPENoteEvent noteEvent{note};
        noteEvent.setStartTime(0.001 * i );
        noteEvent.setReleaseTime(0.001 * (i + 1) );
        notes.push_back(noteEvent);
    }
}

void NoteBars::paint(juce::Graphics& g)
{
    for (auto& note : notes)
    {
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
    double noteStart = note.getStartTime();
    double noteEnd = note.isPlaying() ? noteStart + 0.1 : note.getReleaseTime();
    double noteLength = noteEnd - noteStart;
    auto noteX = static_cast<float>(noteStart * getWidth());
    const int numPitches = 128;
    float noteHeight = (float)getHeight() / numPitches;
    float noteY = static_cast<float>(numPitches - note.getMpeNote().initialNote - 1.0) * noteHeight;
    auto noteWidth = static_cast<float>(noteLength * getWidth()); //TODO: we need to know tempo and time signature here
    return juce::Rectangle<int>{static_cast<int>(noteX), static_cast<int>(noteY), static_cast<int>(noteWidth), static_cast<int>(noteHeight)};
}
