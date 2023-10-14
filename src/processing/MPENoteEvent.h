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
    static constexpr float MAX_NOTE_SHIFT = 1.0f / 2.0f;

    explicit MPENoteEvent(const juce::MPENote& note, size_t index, float noteOnVelocity) :
        mpeNote(note), noteIndex{index}, noteOnVelocity{noteOnVelocity} {}

    MPENoteEvent() = delete;

    MPENoteEvent(const MPENoteEvent& other);

    void setPlayedNoteEvent(const MPENoteEvent& noteEvent);

    void setPpqReleasePosition(const double position);

    double getPpqStartPosition() const;

    double getPpqReleasePosition() const;

    juce::MPENote getMpeNote() const;

    void setPpqStartPosition(const double position);

    juce::String getNoteName() const;

    size_t getNoteIndex() const;

    ~MPENoteEvent() = default;

    MPENoteEvent& operator=(const MPENoteEvent& other);
    
    float getPlayedNoteStartPositionShift() const;

    bool thereIsPlayedNote() const;

    const MPENoteEvent getPlayedNote() const;

    void clearPlayedNote();

    bool isPlayed() const;

    void setIsPlayed(bool isPlayedNote);

    float getNoteOnVelocity() const;

private:
    bool isPlayedNote{false};
    juce::MPENote mpeNote;
    double ppqStartPosition{0};
    double ppqReleasePosition{0};
    size_t noteIndex{0};
    float noteOnVelocity;
    std::unique_ptr<MPENoteEvent> playedNote{nullptr};
};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
