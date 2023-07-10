//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#define PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#include <juce_audio_basics/juce_audio_basics.h>

class MPENoteEvent
{
public:
    MPENoteEvent(const juce::MPENote& note, const juce::Time& start) : mpeNote(note),
            startTime(std::make_unique<juce::Time>(start)), releaseTime(nullptr)
    {
    }
    bool isPlaying() const
    {
        return releaseTime == nullptr;
    }
    void setReleaseTime(const juce::Time& time)
    {
        releaseTime = std::make_unique<juce::Time>(time);
    }
private:
    juce::MPENote mpeNote;
    std::unique_ptr<juce::Time>startTime;
    std::unique_ptr<juce::Time> releaseTime;

};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
