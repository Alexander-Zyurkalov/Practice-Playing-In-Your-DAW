//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#include "NoteGrid.h"

NoteGrid::NoteGrid(juce::Viewport& viewport) : viewport(&viewport)
{
    this->setSize(1400, 3000);
    this->addAndMakeVisible(noteBars);
    this->noteBars.setSize(this->getWidth(), this->getHeight());

}

NoteGrid::~NoteGrid()
{
    // Destructor: Clean up your component here
}

void NoteGrid::paint (juce::Graphics& g)
{
    juce::Rectangle<float> visibleArea = viewport->getViewArea().toFloat();

    int totalBarsInSong = 10;
    int pixelsPerQuarterNote = 100;
    double OneFirstBeatWidth = pixelsPerQuarterNote * 4;
    double beatWidth = OneFirstBeatWidth / timeSignature.getDenominator();
    double barWidth = beatWidth * timeSignature.getNumerator();
    double noteGridWidth = barWidth * totalBarsInSong;
    setSize(static_cast<int>(noteGridWidth), getHeight());
    noteBars.setSize(static_cast<int>(noteGridWidth), getHeight());

    // Set a background color
    g.fillAll (juce::Colours::black);

    // Set the color for the grid lines
    g.setColour (juce::Colours::grey);

    auto bounds = getLocalBounds();

    for (int bar = 0; bar < totalBarsInSong; ++bar)
    {
        for (int beat = 0; beat < timeSignature.getNumerator(); ++beat)
        {
            // Calculate the x position for this beat
            int beatPosition = static_cast<int>(beatWidth * beat + bar * barWidth);
            juce::Line<float> line(beatPosition, 0, beatPosition, bounds.getHeight());
            if (!visibleArea.intersects(line))
                continue;

            // Draw a line at this x position
            g.drawLine (line, 1.0f);

            if (beat == 0)
            {
                // Draw a thicker line at the start of each bar
                g.drawLine (line, 2.0f);
            }
        }
    }

}

void NoteGrid::resized()
{
    // This method is where you'll handle resizing of your component.
}
