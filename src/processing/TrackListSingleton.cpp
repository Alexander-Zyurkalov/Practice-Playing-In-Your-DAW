//
// Created by Alexandr Zyurkalov on 05.07.23.
//

#include "TrackListSingleton.h"

int TrackListSingleton::getNumberOfTracks()
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    return (int)tracks.size();
}

Track& TrackListSingleton::getTrack (size_t index)
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    return tracks[index];
}

void TrackListSingleton::clearTracks()
{
    std::unique_lock<std::shared_mutex> lock(mutex);
    tracks.clear();
}

void TrackListSingleton::addTrack(const Track &track) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    tracks.push_back(track);
}

TrackListSingleton::~TrackListSingleton() {
    clearSingletonInstance();
}

JUCE_IMPLEMENT_SINGLETON (TrackListSingleton)
