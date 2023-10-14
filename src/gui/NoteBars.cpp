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
        g.drawRect(rectangle, 1);
        g.setColour(juce::Colours::grey);
        drawVelocityLine(g, note, rectangle);

        g.setColour(juce::Colours::blue);
        drawMPELine(g, note.getPressures(), rectangle);

        if (note.thereIsPlayedNote()) {
            setColourByShift(g, note.getPlayedNoteStartPositionShift());
            g.beginTransparencyLayer(note.getNoteOnVelocity());
                const juce::Rectangle<int> &playedNoteRectangle = getNoteRectangle(note.getPlayedNote());
                g.fillRect(playedNoteRectangle);
            g.endTransparencyLayer();
            g.drawRect(playedNoteRectangle, 1);

            setColourByShift(g, note.getPlayedNoteVelocityShift());
            drawVelocityLine(g, note.getPlayedNote(), playedNoteRectangle);
        }

        const juce::Rectangle<int> &widthOfText = rectangle.withWidth(rectangle.getHeight()+10);

        
        g.setColour(juce::Colours::black);
        g.drawText(note.getNoteName(), widthOfText, juce::Justification::left);
    }
    middleYPosition = juce::jlimit<float>(0.0f, getHeight(), (maxPitchPosition - minPitchPosition) / 2.0f + minPitchPosition);

}

void NoteBars::setColourByShift(juce::Graphics &g, float shift) const
{
    int dist = static_cast<int>(floor(abs(shift) * 4));
    switch (dist)
    {
        case 0: g.setColour(juce::Colours::green); break;
        case 1: g.setColour(juce::Colours::purple); break;
        case 2:
        case 3:
        default: g.setColour(juce::Colours::red); break;
    }
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
    g.drawLine(originalVelocityLevelLine, 2);
}


void NoteBars::resized()
{
}



juce::Rectangle<int> NoteBars::getNoteRectangle(const MPENoteEvent &note)
{
    double ppqStartPosition = note.getPpqStartPosition() - dawTransportData.getPpqStartLoopPosition();
    double ppqReleasePosition =  note.getPpqReleasePosition() - dawTransportData.getPpqStartLoopPosition();
    double noteLength = ppqReleasePosition - ppqStartPosition;
    if (noteLength <= 0.0)
        noteLength = 1.0/8;
    float noteX = ppqToPixel(ppqStartPosition);
    const int numPitches = 128;
    float noteHeight = (float)getHeight() / numPitches;
    float noteY = static_cast<float>(numPitches - note.getMpeNote().initialNote - 1.0) * noteHeight;
    auto noteWidth = ppqToPixel(noteLength);
    return juce::Rectangle<int>{static_cast<int>(noteX), static_cast<int>(noteY), static_cast<int>(noteWidth), static_cast<int>(noteHeight)};
}

float NoteBars::ppqToPixel(double ppqStartPosition)
{
    int pixelsPerQuarterNote = 100;
    return static_cast<float>(ppqStartPosition * pixelsPerQuarterNote);
}

double NoteBars::pixelToPpq(float pixels)
{
    return pixels * 100.0;
}

float NoteBars::getMiddleYPosition() const
{
    return middleYPosition;
}

void NoteBars::drawMPELine(
        juce::Graphics &graphics, const std::map<double, float> &pressures,
        const juce::Rectangle<int> &rectangle)
{
    juce::Point<float> point{static_cast<float>(rectangle.getBottomLeft().x),
                             static_cast<float>(rectangle.getBottomLeft().y)};
   for (auto &pressure: pressures)
   {
       float x = ppqToPixel(pressure.first);
       float y = static_cast<float>(rectangle.getHeight()) * pressure.second +
                 static_cast<float>(rectangle.getBottomLeft().y);
       juce::Line<float> line{point, juce::Point{x,y}};
       graphics.drawLine(line, 1);
       point = line.getEnd();
   }

}
