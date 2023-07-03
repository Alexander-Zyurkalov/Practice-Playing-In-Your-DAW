//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#include "NoteGrid.h"

NoteGrid::NoteGrid()
{
    // Constructor: Initialize your component here
}

NoteGrid::~NoteGrid()
{
    // Destructor: Clean up your component here
}

void NoteGrid::paint (juce::Graphics& g)
{
    // Set a background color
    g.fillAll (juce::Colours::black);

    // Set the color for the grid lines
    g.setColour (juce::Colours::grey);

    auto bounds = getLocalBounds();
    int totalQuarterNotesInSong = 8;
    double pixelsPerQuarterNote = bounds.getWidth() / totalQuarterNotesInSong;

    int totalBarsInSong = 4;
    for (int bar = 0; bar < totalBarsInSong; ++bar)
    {
        // Assume a 4/4 time signature, so a bar has 4 beats
        for (int beat = 0; beat < 4; ++beat)
        {
            // Calculate the x position for this beat
            int beatPosition = static_cast<int>((bar * 4 + beat) * pixelsPerQuarterNote);

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
