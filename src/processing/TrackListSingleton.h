//
// Created by Alexandr Zyurkalov on 05.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H
#define PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H

#include <juce_core/juce_core.h>

class TrackListSingleton
{
public:

    // Add a track
    void addTrack (const juce::String& track);

    // Get the tracks
    const juce::StringArray& getTracks() const;

    // Clear the tracks
    void clearTracks();

    JUCE_DECLARE_SINGLETON (TrackListSingleton, true)

private:
    TrackListSingleton() = default;

    juce::StringArray tracks;
    static TrackListSingleton* instance;
};


#endif //PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H
