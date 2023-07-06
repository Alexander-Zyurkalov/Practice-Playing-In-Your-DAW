//
// Created by Alexandr Zyurkalov on 05.07.23.
//

#include "TrackListSingleton.h"

void TrackListSingleton::addTrack (const juce::String& track)
{
    tracks.add (track);
}

const juce::StringArray& TrackListSingleton::getTracks() const
{
    return tracks;
}

void TrackListSingleton::clearTracks()
{
    tracks.clear();
}

JUCE_IMPLEMENT_SINGLETON (TrackListSingleton)
