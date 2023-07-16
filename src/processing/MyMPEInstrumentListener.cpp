//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

MyMPEInstrumentListener::MyMPEInstrumentListener(DAWTransportData *transportData): dawTransportData(transportData) {}

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    MPENoteEvent mpeNoteEvent{newNote, noteEventVector.size()};
    mpeNoteEvent.setPpqStartPosition(dawTransportData->getPpqPosition());
    notes.emplace(newNote.noteID, mpeNoteEvent);
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
    MPENoteEvent &event = notes.at(finishedNote.noteID);
    double position = dawTransportData->getPpqPosition();
    event.setPpqReleasePosition(position);
    noteEventVector[event.getNoteIndex()] = event;
    notes.erase(finishedNote.noteID);
}

void MyMPEInstrumentListener::zoneLayoutChanged() {
}

std::vector<MPENoteEvent> MyMPEInstrumentListener::createNoteEventVector() {
    return noteEventVector;
}

void MyMPEInstrumentListener::updateNotes() {
    double position = dawTransportData->getPpqPosition();
    for(auto &note: notes) {
            note.second.setPpqReleasePosition(position);
            noteEventVector[note.second.getNoteIndex()] = note.second;
    }
}
