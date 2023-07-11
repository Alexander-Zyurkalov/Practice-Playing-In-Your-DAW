//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "NoteBars.h"
#include <juce_audio_basics/juce_audio_basics.h>

NoteBars::NoteBars()
{
    setOpaque(false);
    for (int i: {1, 2, 3, 4, 5, 6, 7, 8, 9})
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
        const juce::Point<float> circleCenter(rectangle.getBottomLeft().getX(), rectangle.getCentreY());
        const float circleRadius = rectangle.getHeight() * 1.2f;

        g.setColour(note.isPlaying() ? juce::Colours::red : juce::Colours::lightseagreen);
        g.fillRect(rectangle);

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
    auto noteY = note.getMpeNote().initialNote * getHeight() / 128;
    auto noteWidth = noteLength * getWidth();
    auto noteHeight = getHeight() / 128;
    return juce::Rectangle<int>(noteX, noteY, noteWidth, noteHeight);
}
