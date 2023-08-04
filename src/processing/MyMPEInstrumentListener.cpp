//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

 static double roundPpqPosition(double ppqPosition)  {
    double d = ceil(ppqPosition * 128.0) / 128.0;
    return d;
}

MyMPEInstrumentListener::MyMPEInstrumentListener(DAWTransportData *transportData): dawTransportData(transportData) {}

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    double position = dawTransportData->getPpqPositionNotSynced();
    position = roundPpqPosition(position);

    if (!recording)
    {
        MPENoteEvent *closestNote = findClosestNote(newNote);
        if (closestNote)
        {
            MPENoteEvent playedNote = MPENoteEvent{newNote, closestNote->getNoteIndex()};
            playedNote.setPpqStartPosition(position);
            closestNote->setPlayedNoteEvent(playedNote);
            unfinishedPlayedNotes.emplace(newNote.noteID, playedNote);
        }
        return;
    }
    MPENoteEvent mpeNoteEvent{newNote, noteEventVector.size()};

    mpeNoteEvent.setPpqStartPosition(position);

    // if note is already playing, release it
    if (unfinishedNotes.find(newNote.noteID) != unfinishedNotes.end()) {
        noteReleased(unfinishedNotes.find(newNote.noteID)->second.getMpeNote());
    }

    unfinishedNotes.emplace(newNote.noteID, mpeNoteEvent);
    noteEventVector.push_back(mpeNoteEvent);

}

MPENoteEvent *MyMPEInstrumentListener::findClosestNote(const juce::MPENote &newNote)
{
    double position = dawTransportData->getPpqPositionNotSynced();
    position = roundPpqPosition(position);
    double minDistance = MPENoteEvent::MAX_NOTE_SHIFT;
    MPENoteEvent* closestNote = nullptr;
    for (auto& noteEvent: noteEventVector)
    {
        if (noteEvent.getMpeNote().noteID == newNote.noteID)
        {
            double distance = std::abs(noteEvent.getPpqStartPosition() - position);
            if (distance < minDistance)
            {
                minDistance = distance;
                closestNote = &noteEvent;
            }
        }
    }
    return closestNote;
}

void MyMPEInstrumentListener::notePressureChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::notePitchbendChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::noteTimbreChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::noteKeyStateChanged(juce::MPENote changedNote) {
}

static MPENoteEvent& findMPENoteEvent(std::unordered_map<juce::uint16 , MPENoteEvent>& unfinishedNotes,
                                      juce::uint16 noteID,
                                      std::vector<MPENoteEvent>& noteEventVector,
                                      DAWTransportData* dawTransportData

                  ) {
    size_t noteIndex = unfinishedNotes.at(noteID).getNoteIndex();
    MPENoteEvent& event = noteEventVector[noteIndex];
    unfinishedNotes.erase(noteID);
    return event;
}

void MyMPEInstrumentListener::noteReleased(juce::MPENote finishedNote) {
    double position = dawTransportData->getPpqPositionNotSynced();
    if (unfinishedNotes.find(finishedNote.noteID) != unfinishedNotes.end())
    {
        MPENoteEvent &event = findMPENoteEvent(unfinishedNotes, finishedNote.noteID, noteEventVector, dawTransportData);
        event.setPpqReleasePosition(position);
    }
    if (unfinishedPlayedNotes.find(finishedNote.noteID) != unfinishedPlayedNotes.end())
    {
        MPENoteEvent &event = findMPENoteEvent(unfinishedPlayedNotes, finishedNote.noteID, noteEventVector,
                                               dawTransportData);
        MPENoteEvent playedEvent = event.getPlayedNote();
        playedEvent.setPpqReleasePosition(position);
        event.setPlayedNoteEvent(playedEvent);
    }

}

void MyMPEInstrumentListener::zoneLayoutChanged() {
}

std::vector<MPENoteEvent> MyMPEInstrumentListener::createNoteEventVector() {
    return noteEventVector; //TODO: replace with deep copy
}

void MyMPEInstrumentListener::updateNotes(double ppqPosition) {
    if (justStartedRecording ){
        clearRecordedNotes();
        justStartedRecording = false;
    }
    ppqPosition = roundPpqPosition(ppqPosition);
    for(auto &note: unfinishedNotes) {
            note.second.setPpqReleasePosition(ppqPosition);
            noteEventVector[note.second.getNoteIndex()] = note.second;
    }
    for(auto &note: unfinishedPlayedNotes) {
        note.second.setPpqReleasePosition(ppqPosition);
        noteEventVector[note.second.getNoteIndex()].setPlayedNoteEvent(note.second);
    }
}


void MyMPEInstrumentListener::toggleRecording() {
    if (!recording)
        justStartedRecording = true;
    recording = !recording;
}

bool MyMPEInstrumentListener::isRecording() const {
    return recording;
}

void MyMPEInstrumentListener::clearRecordedNotes() {
    noteEventVector.clear();
    unfinishedNotes.clear();
    unfinishedPlayedNotes.clear();
}

bool MyMPEInstrumentListener::isJustStartedRecording() const {
    return justStartedRecording;
}

