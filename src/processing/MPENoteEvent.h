//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#define PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#include <juce_audio_basics/juce_audio_basics.h>

class MPENoteEvent
{
public:
    MPENoteEvent(const juce::MPENote& note) : mpeNote(note) {}

    bool isPlaying() const
    {
        return releaseTime < startTime;
    }
    void setReleaseTime(const double time)
    {
        releaseTime =time;
    }

    double getStartTime() const
    {
        return startTime;
    }

    double getReleaseTime() const
    {
        return releaseTime;
    }

    juce::MPENote getMpeNote() const
    {
        return mpeNote;
    }

    void setStartTime(const double time)
    {
        startTime = time;
    }

    juce::String getNoteName() const
    {
        return juce::MidiMessage::getMidiNoteName(mpeNote.initialNote, true, true, 3);
    }
private:
    juce::MPENote mpeNote;
    double startTime;
    double releaseTime;

};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
