//
// Created by Alexandr Zyurkalov on 06.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_TRACKLISTBOXMODEL_H
#define PRACTICEPLAYINGINDAW_TRACKLISTBOXMODEL_H
#include "../processing/TrackListSingleton.h"
#include "juce_gui_basics/juce_gui_basics.h"

class TrackListBoxModel : public juce::ListBoxModel
{
public:
    explicit TrackListBoxModel(const Track track ): instanceTrack(track) {}
    ~TrackListBoxModel() override = default;

    // This should return the total number of rows in the table.
    int getNumRows() override
    {
        return trackListSingleton->getNumberOfTracks();
    }

    // This is overloaded from ListBoxModel, and should fill in the background of the whole row
    void paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) override
    {
        if (rowNumber == instanceTrack.getId())
            g.fillAll (juce::Colours::lightblue);
        else
            g.fillAll (juce::Colours::grey);

        g.setColour (juce::Colours::black);
        g.setFont (height * 0.7f);
        juce::StringArray tracks;
        for (int i = 0; i < trackListSingleton->getNumberOfTracks(); ++i)
        {
            tracks.add (trackListSingleton->getTrack(i).getName());
        }


        if (rowNumber < tracks.size())
        {
            g.drawText (tracks.getReference(rowNumber),
                        5, 0, width, height,
                        juce::Justification::centredLeft, true);
        }
    }

private:
    TrackListSingleton* trackListSingleton = TrackListSingleton::getInstance();
    Track instanceTrack;
};



#endif //PRACTICEPLAYINGINDAW_TRACKLISTBOXMODEL_H
