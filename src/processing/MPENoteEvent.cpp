//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MPENoteEvent.h"

MPENoteEvent::MPENoteEvent(const MPENoteEvent& other): mpeNote(other.mpeNote), ppqStartPosition(other.ppqStartPosition),
                                                       ppqReleasePosition(other.ppqReleasePosition), noteIndex{other.noteIndex}
{
    if (other.playedNote != nullptr)
        playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
}

void MPENoteEvent::setPlayedNoteEvent(const MPENoteEvent &noteEvent)
{
    playedNote = std::make_unique<MPENoteEvent>(noteEvent);
}

void MPENoteEvent::setPpqReleasePosition(const double position)
{
    ppqReleasePosition = position;
}

double MPENoteEvent::getPpqStartPosition() const
{
    return ppqStartPosition;
}

double MPENoteEvent::getPpqReleasePosition() const
{
    return ppqReleasePosition;
}

juce::MPENote MPENoteEvent::getMpeNote() const
{
    return mpeNote;
}

void MPENoteEvent::setPpqStartPosition(const double position)
{
    ppqStartPosition = position;
}

juce::String MPENoteEvent::getNoteName() const
{
    return juce::MidiMessage::getMidiNoteName(mpeNote.initialNote, true, true, 3);
}

size_t MPENoteEvent::getNoteIndex() const
{
    return noteIndex;
}

MPENoteEvent &MPENoteEvent::operator=(const MPENoteEvent &other)
{
    if (this == &other)
    {
        return *this;
    }
    mpeNote = other.mpeNote;
    ppqStartPosition = other.ppqStartPosition;
    ppqReleasePosition = other.ppqReleasePosition;
    noteIndex = other.noteIndex;
    if (other.playedNote != nullptr)
        playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
    return *this;
}

bool MPENoteEvent::playedNoteIsTheSame() const
{
    if (playedNote != nullptr)
        return playedNote->getMpeNote() == mpeNote &&
               std::abs(playedNote->getPpqStartPosition() - ppqStartPosition) < 1.0/128.0 &&
               std::abs(playedNote->getPpqReleasePosition() - ppqReleasePosition) < 1.0/128.0;
    return true;
}

float MPENoteEvent::getPlayedNoteStartPositionShift() const
{

    return std::clamp(static_cast<float>(std::abs(ppqStartPosition - playedNote->getPpqStartPosition())), 0.0f, MAX_NOTE_SHIFT) * 1 / MAX_NOTE_SHIFT;
}

bool MPENoteEvent::thereIsPlayedNote() const
{
    return playedNote != nullptr;
}

const MPENoteEvent MPENoteEvent::getPlayedNote() const
{
    return playedNote ? *playedNote : MPENoteEvent(mpeNote, noteIndex);
}

void MPENoteEvent::clearPlayedNote()
{
    playedNote.reset();
}
