//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MPENoteEvent.h"

MPENoteEvent::MPENoteEvent(const MPENoteEvent& other): mpeNote(other.mpeNote), ppqStartPosition(other.ppqStartPosition),
                                                       ppqReleasePosition(other.ppqReleasePosition), noteIndex{other.noteIndex}
{
    if (other.playedNote)
    {
        playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
    }
}

void MPENoteEvent::setPlayedNoteEvent(const MPENoteEvent &noteEvent)
{
    playedNote = std::make_unique<MPENoteEvent>(noteEvent);
    // temporary just  slightly shift ppqStartPosition to the left and ppqReleasePosition to the right
    playedNote->setPpqStartPosition(ppqStartPosition - 0.1);
    playedNote->setPpqReleasePosition(ppqReleasePosition + 0.1);
}

void MPENoteEvent::setPpqReleasePosition(const double position)
{
    ppqReleasePosition = position;
    // temporary set ppqReleasePosition for playedNote slightly to the right
    if (playedNote)
    {
        playedNote->setPpqReleasePosition(position + 0.1);
    }

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
    // temporary set ppqStartPosition for playedNote slightly to the left
    if (playedNote)
    {
        playedNote->setPpqStartPosition(position - 0.1);
    }
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
    if (other.playedNote)
    {
        playedNote = std::make_unique<MPENoteEvent>(*other.playedNote);
    }
    return *this;
}
