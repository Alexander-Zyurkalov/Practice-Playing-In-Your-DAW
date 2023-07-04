//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#include "NoteGrid.h"

NoteGrid::NoteGrid()
{
    this->setSize(1400, 1200);
}

NoteGrid::~NoteGrid()
{
    // Destructor: Clean up your component here
}

void NoteGrid::paint (juce::Graphics& g)
{
    int totalBarsInSong = 10;
    int pixelsPerQuarterNote = 100;
    double OneFirstBeatWidth = pixelsPerQuarterNote * 4;
    double beatWidth = OneFirstBeatWidth / timeSignature.getDenominator();
    double barWidth = beatWidth * timeSignature.getNumerator();
    double noteGridWidth = barWidth * totalBarsInSong;
    setSize(static_cast<int>(noteGridWidth), 1200);

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

            // Draw a line at this x position
            g.drawLine (beatPosition, 0, beatPosition, bounds.getHeight(), 1.0f);

            if (beat == 0)
            {
                // Draw a thicker line at the start of each bar
                g.drawLine (beatPosition, 0, beatPosition, bounds.getHeight(), 2.0f);
            }
        }
    }
}

void NoteGrid::resized()
{
    // This method is where you'll handle resizing of your component.
}
