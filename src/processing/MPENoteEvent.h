//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#define PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
#include <juce_audio_basics/juce_audio_basics.h>
#include <memory>
class MPENoteEvent
{
public:
    explicit MPENoteEvent(const juce::MPENote& note, size_t index) : mpeNote(note), noteIndex{index} {}

    MPENoteEvent() = delete;

    MPENoteEvent(const MPENoteEvent& other) : mpeNote(other.mpeNote), ppqStartPosition(other.ppqStartPosition),
                                              ppqReleasePosition(other.ppqReleasePosition), noteIndex{other.noteIndex}
    {
        if (other.playedNote)
        {
            playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
        }
    }

    void setPlayedNoteEvent(const MPENoteEvent& noteEvent)
    {
        playedNote = std::make_unique<MPENoteEvent>(noteEvent);
        // temporary just  slightly shift ppqStartPosition to the left and ppqReleasePosition to the right
        playedNote->setPpqStartPosition(ppqStartPosition - 0.1);
        playedNote->setPpqReleasePosition(ppqReleasePosition + 0.1);
    }

    void setPpqReleasePosition(const double position)
    {
        ppqReleasePosition = position;
        // temporary set ppqReleasePosition for playedNote slightly to the right
        if (playedNote)
        {
            playedNote->setPpqReleasePosition(position + 0.1);
        }

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
        // temporary set ppqStartPosition for playedNote slightly to the left
        if (playedNote)
        {
            playedNote->setPpqStartPosition(position - 0.1);
        }
        ppqStartPosition = position;
    }

    juce::String getNoteName() const
    {
        return juce::MidiMessage::getMidiNoteName(mpeNote.initialNote, true, true, 3);
    }

    size_t getNoteIndex() const {
        return noteIndex;
    }

    ~MPENoteEvent() = default;

    MPENoteEvent& operator=(const MPENoteEvent& other)
    {
        if (this == &other)
        {
            return *this;
        }
        mpeNote = other.mpeNote;
        ppqStartPosition = other.ppqStartPosition;
        ppqReleasePosition = other.ppqReleasePosition;
        noteIndex = other.noteIndex;
        if (other.playedNote)
        {
            playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
        }
        return *this;
    }

private:

    juce::MPENote mpeNote;
    double ppqStartPosition{0};
    double ppqReleasePosition{0};
    size_t noteIndex{0};
    std::unique_ptr<MPENoteEvent> playedNote{nullptr};
};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
