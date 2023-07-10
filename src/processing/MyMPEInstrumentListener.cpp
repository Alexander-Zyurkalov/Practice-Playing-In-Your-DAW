//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#include "MyMPEInstrumentListener.h"

void MyMPEInstrumentListener::noteAdded(juce::MPENote newNote) {
    Listener::noteAdded(newNote);
    notes.emplace(newNote.noteID,
                    MPENoteEvent{newNote});

}

void MyMPEInstrumentListener::notePressureChanged(juce::MPENote changedNote) {
    Listener::notePressureChanged(changedNote);
}

void MyMPEInstrumentListener::notePitchbendChanged(juce::MPENote changedNote) {
    Listener::notePitchbendChanged(changedNote);
}

void MyMPEInstrumentListener::noteTimbreChanged(juce::MPENote changedNote) {
    Listener::noteTimbreChanged(changedNote);
}

void MyMPEInstrumentListener::noteKeyStateChanged(juce::MPENote changedNote) {
    Listener::noteKeyStateChanged(changedNote);
}

void MyMPEInstrumentListener::noteReleased(juce::MPENote finishedNote) {
    Listener::noteReleased(finishedNote);
    notes.at(finishedNote.noteID).setReleaseTime(1);
}

void MyMPEInstrumentListener::zoneLayoutChanged() {
    Listener::zoneLayoutChanged();
}
