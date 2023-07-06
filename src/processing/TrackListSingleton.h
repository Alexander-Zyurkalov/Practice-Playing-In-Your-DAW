//
// Created by Alexandr Zyurkalov on 05.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H
#define PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H

#include <juce_core/juce_core.h>
#include "Track.h"
#include <vector>
#include <shared_mutex>

class TrackListSingleton
{
public:

    // Add a track
    void addTrack (const Track& track);

    virtual ~TrackListSingleton();

    int getNumberOfTracks();

    void deleteById (int id);

    std::vector<Track> getTracks();

    JUCE_DECLARE_SINGLETON (TrackListSingleton, false)

private:
    TrackListSingleton() = default;

    std::vector<Track> tracks;

    static TrackListSingleton* instance;

    std::shared_mutex mutex;
};


#endif //PRACTICEPLAYINGINDAW_TRACKLISTSINGLETON_H
