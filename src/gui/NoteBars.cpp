//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "NoteBars.h"
#include <juce_audio_basics/juce_audio_basics.h>

NoteBars::NoteBars(NoteGridViewPort& viewport): viewport(&viewport)
{
    setOpaque(false);
}

void NoteBars::paint(juce::Graphics& g)
{
    juce::Rectangle<float> visibleArea = viewport->getViewArea().withBottomY(0).withBottom(getHeight()).toFloat();
    int pixelsPerQuarterNote = 100; //TODO: it is the second place where we use this constant. Do something with that, DRY!!!
    double ppqLeftPosition = dawTransportData.getPpqStartLoopPosition();
    double ppqRightPosition = dawTransportData.getPpqEndLoopPosition() + (double)getHeight()/pixelsPerQuarterNote;

    int minPitchPosition = getHeight();
    int maxPitchPosition = 0.0f;
    for (auto& note : notes)
    {
        if (ppqLeftPosition < note.getPpqStartPosition() && ppqRightPosition < note.getPpqStartPosition())
            continue;
        if (ppqLeftPosition> note.getPpqReleasePosition() && ppqRightPosition > note.getPpqReleasePosition())
            continue;

        const juce::Rectangle<int>& rectangle = getNoteRectangle(note);
        if (!visibleArea.intersects(rectangle.toFloat()))
            continue;

        if (minPitchPosition > rectangle.getPosition().y)
            minPitchPosition = rectangle.getPosition().y;
        if (maxPitchPosition < rectangle.getPosition().y + rectangle.getHeight())
            maxPitchPosition = rectangle.getPosition().y + rectangle.getHeight();

        g.setColour(juce::Colours::lightskyblue);
        g.beginTransparencyLayer(note.getNoteOnVelocity());
            g.fillRect(rectangle);
        g.endTransparencyLayer();
        drawVelocityLine(g, note, rectangle);
        g.drawRect(rectangle, 1);

        if (note.thereIsPlayedNote()) {
            int dist = static_cast<int>(std::floor(std::abs(note.getPlayedNoteStartPositionShift()) * 4));
            switch (dist)
            {
                case 0: g.setColour(juce::Colours::green); break;
                case 1: g.setColour(juce::Colours::purple); break;
                case 2:
                case 3:
                default: g.setColour(juce::Colours::red); break;
            }
            g.beginTransparencyLayer(note.getNoteOnVelocity());
                const juce::Rectangle<int> &playedNoteRectangle = getNoteRectangle(note.getPlayedNote());
                g.fillRect(playedNoteRectangle);
            g.endTransparencyLayer();
            drawVelocityLine(g, note.getPlayedNote(), playedNoteRectangle);
            g.drawRect(playedNoteRectangle, 1);
        }

        const juce::Rectangle<int> &widthOfText = rectangle.withWidth(rectangle.getHeight()+10);

        
        g.setColour(juce::Colours::black);
        g.drawText(note.getNoteName(), widthOfText, juce::Justification::left);
    }
    middleYPosition = juce::jlimit<float>(0.0f, getHeight(), (maxPitchPosition - minPitchPosition) / 2.0f + minPitchPosition);

}

void
NoteBars::drawVelocityLine(const juce::Graphics &g, const MPENoteEvent &note,
                           const juce::Rectangle<int> &rectangle) const
{
    const float w = static_cast<const float>(rectangle.getTopRight().y - rectangle.getBottomLeft().y);
    float y = static_cast<float>(rectangle.getBottomLeft().y) + w * note.getNoteOnVelocity();
    juce::Point<float> leftPoint{static_cast<float>(rectangle.getBottomLeft().x), y};
    juce::Point<float> rightPoint{static_cast<float>(rectangle.getBottomRight().x), y};
    juce::Line<float> originalVelocityLevelLine{leftPoint, rightPoint};
    g.drawLine(originalVelocityLevelLine);
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

float NoteBars::getMiddleYPosition() const
{
    return middleYPosition;
}
