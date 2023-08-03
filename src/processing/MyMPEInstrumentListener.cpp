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
    double minDistance = 1.0;
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

static MPENoteEvent& processNoteRelease(std::unordered_map<juce::uint16 , MPENoteEvent>& unfinishedNotes,
                  juce::MPENote& finishedNote,
                  std::vector<MPENoteEvent>& noteEventVector,
                  DAWTransportData* dawTransportData

                  ) {
    MPENoteEvent &event = unfinishedNotes.at(finishedNote.noteID);
    double position = dawTransportData->getPpqPositionNotSynced();
    position = roundPpqPosition(position);
    event.setPpqReleasePosition(position);
    unfinishedNotes.erase(finishedNote.noteID);
    return event;
}

void MyMPEInstrumentListener::noteReleased(juce::MPENote finishedNote) {
    if (unfinishedNotes.find(finishedNote.noteID) != unfinishedNotes.end())
    {
        MPENoteEvent &event = processNoteRelease(unfinishedNotes, finishedNote, noteEventVector, dawTransportData);
        if (event.getNoteIndex() < noteEventVector.size())
        {
            noteEventVector[event.getNoteIndex()] = event;
        }
    }
    if (unfinishedPlayedNotes.find(finishedNote.noteID) != unfinishedPlayedNotes.end())
    {
        MPENoteEvent &playedEvent = processNoteRelease(unfinishedPlayedNotes, finishedNote, noteEventVector,
                                                       dawTransportData);
        noteEventVector[playedEvent.getNoteIndex()].setPlayedNoteEvent(playedEvent);
    }

}

void MyMPEInstrumentListener::zoneLayoutChanged() {
}

std::vector<MPENoteEvent> MyMPEInstrumentListener::createNoteEventVector() {
    return noteEventVector;
}

void MyMPEInstrumentListener::updateNotes(double ppqPosition) {
    if (justStartedRecording ){
        auto& td = dawTransportData;

        clearRecordedNotes();
        justStartedRecording = false;
    }
    ppqPosition = roundPpqPosition(ppqPosition);
    for(auto &note: unfinishedNotes) {
            note.second.setPpqReleasePosition(ppqPosition);
            noteEventVector[note.second.getNoteIndex()] = note.second;
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
}

bool MyMPEInstrumentListener::isJustStartedRecording() const {
    return justStartedRecording;
}

