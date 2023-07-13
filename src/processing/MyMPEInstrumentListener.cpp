//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

MyMPEInstrumentListener::MyMPEInstrumentListener(DAWTransportData *transportData): dawTransportData(transportData) {}

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    MPENoteEvent mpeNoteEvent{newNote};
    mpeNoteEvent.setPpqStartPosition(dawTransportData->getPpqPosition());
    notes.emplace(newNote.noteID, MPENoteEvent{newNote});

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
    notes.at(finishedNote.noteID).setPpqReleasePosition(dawTransportData->getPpqPosition());
}

void MyMPEInstrumentListener::zoneLayoutChanged() {
}

std::vector<MPENoteEvent> MyMPEInstrumentListener::createNoteEventVector() {
    std::vector<MPENoteEvent> noteEventVector;
    noteEventVector.reserve(notes.size());
    for (auto& note: notes) {
        noteEventVector.push_back(note.second);
    }
    return noteEventVector;
}