//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

 double MyMPEInstrumentListener::roundPpqPosition(double ppqPosition)  {
    double d = ceil(ppqPosition * 128.0) / 128.0;
    return d;
}

MyMPEInstrumentListener::MyMPEInstrumentListener(DAWTransportData *transportData): dawTransportData(transportData) {}

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    if (!recording)
        return;
    MPENoteEvent mpeNoteEvent{newNote, noteEventVector.size()};
    double position = dawTransportData->getPpqPositionNotSynced();
    position = roundPpqPosition(position);
    mpeNoteEvent.setPpqStartPosition(position);

    if (unfinishedNotes.find(newNote.noteID) != unfinishedNotes.end()) {
        noteReleased(unfinishedNotes.find(newNote.noteID)->second.getMpeNote());
    }
    unfinishedNotes.emplace(newNote.noteID, mpeNoteEvent);
    noteEventVector.push_back(mpeNoteEvent);

}

void MyMPEInstrumentListener::notePressureChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::notePitchbendChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::noteTimbreChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::noteKeyStateChanged(juce::MPENote changedNote) {
}

void MyMPEInstrumentListener::noteReleased(juce::MPENote finishedNote) {
    if (unfinishedNotes.find(finishedNote.noteID) == unfinishedNotes.end())
        return;
    MPENoteEvent &event = unfinishedNotes.at(finishedNote.noteID);
    double position = dawTransportData->getPpqPositionNotSynced();
    position = roundPpqPosition(position);
    event.setPpqReleasePosition(position);
    if (event.getNoteIndex() < noteEventVector.size())
        noteEventVector[event.getNoteIndex()] = event;
    unfinishedNotes.erase(finishedNote.noteID);
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

