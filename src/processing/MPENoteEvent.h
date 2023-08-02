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
    
    bool playedNoteIsTheSame() const;

    float getPlayedNoteStartPositionShift() const;

    bool thereIsPlayedNote() const;

    const MPENoteEvent getPlayedNote() const;

private:

    juce::MPENote mpeNote;
    double ppqStartPosition{0};
    double ppqReleasePosition{0};
    size_t noteIndex{0};
    std::unique_ptr<MPENoteEvent> playedNote{nullptr};
};




#endif //PRACTICEPLAYINGINDAW_MPENOTEEVENT_H
