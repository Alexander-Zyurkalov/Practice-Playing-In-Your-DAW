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
    double position = dawTransportData->getPpqPosition();
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

void MyMPEInstrumentListener::updateNotes() {
    if (clearBeforeRecording){
        auto& td = dawTransportData;
        auto iter = std::remove_if(noteEventVector.begin(), noteEventVector.end(),
               [&td](auto& note)
               {

                    return (note.getPpqStartPosition() >= td->getPpqPosition()) &&
                            (note.getPpqStartPosition() <= td->getPpqEndLoopPosition());
               }
        );
        noteEventVector.erase(iter);
        clearBeforeRecording = false;
    }
    double position = dawTransportData->getPpqPosition();
    for(auto &note: unfinishedNotes) {
            note.second.setPpqReleasePosition(position);
            noteEventVector[note.second.getNoteIndex()] = note.second;
    }
}

void MyMPEInstrumentListener::toggleRecording() {
    if (!recording)
        clearBeforeRecording = true;
    recording = !recording;
}

bool MyMPEInstrumentListener::isRecording() const {
    return recording;
}

void MyMPEInstrumentListener::clearRecordedNotes() {
    noteEventVector.clear();
    unfinishedNotes.clear();
}
