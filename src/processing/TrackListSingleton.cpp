//
// Created by Alexandr Zyurkalov on 05.07.23.
//

#include "TrackListSingleton.h"

int TrackListSingleton::getNumberOfTracks()
{
    std::shared_lock<std::shared_mutex> lock(mutex);
    return (int)tracks.size();
}


void TrackListSingleton::addTrack(const Track &track) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    tracks.push_back(track);
}

TrackListSingleton::~TrackListSingleton() {
    clearSingletonInstance();
}


void TrackListSingleton::deleteById (int id)
{
    std::unique_lock<std::shared_mutex> lock(mutex);
    std::erase_if(tracks, [id](const Track& track) { return track.getId() == id; });
}

std::vector<Track> TrackListSingleton::getTracks() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return tracks;
}

JUCE_IMPLEMENT_SINGLETON (TrackListSingleton)
