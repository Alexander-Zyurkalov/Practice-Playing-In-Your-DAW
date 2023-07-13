//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#define PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#include <juce_audio_basics/juce_audio_basics.h>

class MPENoteEvent
{
public:
    explicit MPENoteEvent(const juce::MPENote& note) : mpeNote(note) {}

    MPENoteEvent() = default;

    bool isPlaying() const
    {
        return ppqReleasePosition < ppqStartPosition;
    }
    void setPpqReleasePosition(const double position)
    {
        ppqReleasePosition = position;
    }

    double getPpqStartPosition() const
    {
        return ppqStartPosition;
    }

    double getPpqReleasePosition() const
    {
        return ppqReleasePosition;
    }

    juce::MPENote getMpeNote() const
    {
        return mpeNote;
    }

    void setPpqStartPosition(const double position)
    {
        ppqStartPosition = position;
    }

    juce::String getNoteName() const
    {
        return juce::MidiMessage::getMidiNoteName(mpeNote.initialNote, true, true, 3);
    }
private:
    juce::MPENote mpeNote;
    double ppqStartPosition{0};
    double ppqReleasePosition{0};

};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
