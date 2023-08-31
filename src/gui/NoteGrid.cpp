//
// Created by Alexandr Zyurkalov on 03.07.23.
//

#include "NoteGrid.h"

bool isSharp(int pitch)
{
    int pitchClass = pitch % 12;
    return pitchClass == 1 || pitchClass == 3 || pitchClass == 6 || pitchClass == 8 || pitchClass == 10;
}

NoteGrid::NoteGrid(NoteGridViewPort& viewport) : viewport(&viewport), noteBars(viewport)
{
    this->setSize(3000, 3000);
    this->addAndMakeVisible(noteBars);
    this->noteBars.setSize(this->getWidth(), this->getHeight());

}

NoteGrid::~NoteGrid()
{
    // Destructor: Clean up your component here
}


void NoteGrid::paint(juce::Graphics& g)
{
    juce::Rectangle<float> visibleArea = viewport->getViewArea().toFloat();
    int pixelsPerQuarterNote = 100; //TODO: define it as constant or do scaling with this variable



    // Set a background color
    g.fillAll(juce::Colours::lightgrey);

    // Draw pitch lines
    g.setColour(juce::Colours::grey);

    const int numPitches = 128;
    const float pitchHeight = (float)getHeight() / numPitches;

    for (int pitch = 0; pitch < numPitches; ++pitch)
    {
        float pitchPosition = (float)(numPitches - pitch - 1) * pitchHeight;

        juce::Line<float> pitchLine(0, pitchPosition, static_cast<float>(getWidth()), pitchPosition);
        juce::Rectangle<float> pitchArea(0, pitchPosition, static_cast<float>(getWidth()), pitchHeight);

        if (visibleArea.intersects(pitchArea))
        {
            if (isSharp(pitch))
            {
                // Fill the area with a darker color for sharps
                g.setColour(juce::Colour(199, 199, 199));
                g.fillRect(pitchArea);
            }
            g.setColour(juce::Colour(180, 180, 180));
            int thickness = (pitch+1) % 12 == 0 ? 5 : 1;
            g.drawLine(pitchLine, thickness);
        }
    }

    // Set the color for the grid lines
    g.setColour(juce::Colours::grey);

    int totalBarsInSong = dawTransportData.getNumBars();
    totalBarsInSong = totalBarsInSong == 0 ? 1 : totalBarsInSong;
    double quarterNotesPerBeat = 4.0 / dawTransportData.getDenominator();
    double beatWidth = quarterNotesPerBeat * pixelsPerQuarterNote;
    double barWidth = beatWidth * dawTransportData.getNumerator();
    double noteGridWidth = barWidth * totalBarsInSong;
    setSize(static_cast<int>(noteGridWidth), getHeight());
    noteBars.setSize(static_cast<int>(noteGridWidth), getHeight());

    auto bounds = getLocalBounds();
    double ppqPosition = dawTransportData.getPpqStartLoopPosition();
    


    for (int bar = 0; bar < totalBarsInSong; ++bar)
    {
        for (int beat = 0; beat < dawTransportData.getNumerator(); ++beat)
        {
            // Calculate the x position for this beat
            int beatPosition = static_cast<int>(beatWidth * beat + bar * barWidth);
            bool isBar = beat == 0;
            drawTheCell(beatPosition, isBar, g, visibleArea, bounds);
        }
    }

    auto cursorX = static_cast<float>(dawTransportData.getPpqPosition() * pixelsPerQuarterNote);
    float contentHeight = static_cast<float>(viewport->getViewedComponent()->getHeight());
    if (cursorX >= 0 && cursorX <= static_cast<float>(getWidth()))
    {
        g.setColour(juce::Colours::red);
        juce::Line<float> line{cursorX, 0, cursorX, contentHeight};
        g.drawLine(line, 2.0f);
    }
    float viewportHeight = static_cast<float>(viewport->getViewHeight());
    float xProportion = cursorX / static_cast<float>(getWidth());
    float yProportion = noteBars.getMiddleYPosition() / contentHeight;

    viewport->setViewPositionProportionately(xProportion, yProportion);


}




void NoteGrid::resized()
{
    // This method is where you'll handle resizing of your component.
}

void NoteGrid::updateDAWTransportData(DAWTransportData transportData)
{
    dawTransportData = transportData;
    noteBars.updateDAWTransportData(transportData);
    repaint();
}

void NoteGrid::drawTheCell(int beatPosition, bool isBar, juce::Graphics &g, juce::Rectangle<float> &visibleArea,
                           juce::Rectangle<int> bounds)
{
    juce::Line<float> line(static_cast<float>(beatPosition), 0, static_cast<float>(beatPosition),
                           static_cast<float>(bounds.getHeight()));
    if (!visibleArea.intersects(line))
        return;

    // Draw a line at this x position
    g.drawLine(line, 1.0f);

    if (isBar)
    {
        // Draw a thicker line at the start of each bar
        g.setColour(juce::Colour(180, 180, 180));
        g.drawLine(line, 5.0f);
    }

}


