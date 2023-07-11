//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "NoteBars.h"
#include <juce_audio_basics/juce_audio_basics.h>

NoteBars::NoteBars()
{
    setOpaque(false);
    for (int i: {1, 2, 3, 4, 5, 6, 7, 8, 9, -10})
    {
        const juce::MPENote &note = juce::MPENote(1, 60 + i, juce::MPEValue(), juce::MPEValue(),
                                                  juce::MPEValue(), juce::MPEValue(),juce::MPENote::KeyState::keyDown);
        MPENoteEvent noteEvent{note};
        noteEvent.setStartTime(0.1 * i + 0.1 * juce::Random::getSystemRandom().nextDouble());
        noteEvent.setReleaseTime(0.1 * (i + 1) + 0.1 * juce::Random::getSystemRandom().nextDouble());
        notes.push_back(noteEvent);
    }
}

void NoteBars::paint(juce::Graphics& g)
{
    for (auto& note : notes)
    {
        const juce::Rectangle<float>& rectangle = getNoteRectangle(note).toFloat();

        g.setColour( juce::Colours::lightskyblue);
        g.fillRect(rectangle);
        g.setColour(juce::Colours::black);
        g.drawRect(rectangle, 0.2f);

        const juce::Rectangle<float> &widthOfText = rectangle.withWidth(rectangle.getHeight());

        g.setColour(juce::Colours::black);
        g.drawText(note.getNoteName(), widthOfText, juce::Justification::left);
    }
}


void NoteBars::resized()
{
}

juce::Rectangle<int> NoteBars::getNoteRectangle(const MPENoteEvent &note)
{
    auto noteStart = note.getStartTime();
    auto noteEnd = note.isPlaying() ? noteStart + 0.1 : note.getReleaseTime();
    auto noteLength =noteEnd - noteStart;
    auto noteX = noteStart * getWidth();
    const int numPitches = 128;
    auto noteY = note.getMpeNote().initialNote * getHeight() / numPitches;
    auto noteWidth = noteLength * getWidth();
    auto noteHeight = getHeight() / numPitches;
    return juce::Rectangle<int>(noteX, noteY, noteWidth, noteHeight);
}
