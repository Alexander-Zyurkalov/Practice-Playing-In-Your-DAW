//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

MyMPEInstrumentListener::MyMPEInstrumentListener(DAWTransportData *transportData): dawTransportData(transportData) {}

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    if (!recording)
        return;
    MPENoteEvent mpeNoteEvent{newNote, noteEventVector.size()};
    mpeNoteEvent.setPpqStartPosition(dawTransportData->getPpqPosition());
    if (recordedNotes.find(newNote.noteID) != recordedNotes.end()) {
        noteReleased(recordedNotes.find(newNote.noteID)->second.getMpeNote());
    }
    recordedNotes.emplace(newNote.noteID, mpeNoteEvent);
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
    if (recordedNotes.find(finishedNote.noteID) == recordedNotes.end())
        return;
    MPENoteEvent &event = recordedNotes.at(finishedNote.noteID);
    double position = dawTransportData->getPpqPosition();
    event.setPpqReleasePosition(position);
    noteEventVector[event.getNoteIndex()] = event;
    recordedNotes.erase(finishedNote.noteID);
}

void MyMPEInstrumentListener::zoneLayoutChanged() {
}

std::vector<MPENoteEvent> MyMPEInstrumentListener::createNoteEventVector() {
    return noteEventVector;
}

void MyMPEInstrumentListener::updateNotes() {
    double position = dawTransportData->getPpqPosition();
    for(auto &note: recordedNotes) {
            note.second.setPpqReleasePosition(position);
            noteEventVector[note.second.getNoteIndex()] = note.second;
    }
}

void MyMPEInstrumentListener::toggleRecording() {
    recording = !recording;
}

bool MyMPEInstrumentListener::isRecording() const {
    return recording;
}
