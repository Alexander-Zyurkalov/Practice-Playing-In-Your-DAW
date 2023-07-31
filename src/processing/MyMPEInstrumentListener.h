//
// Created by Alexandr Zyurkalov on 10.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_MYMPEINSTRUMENTLISTENER_H
#define PRACTICEPLAYINGINDAW_MYMPEINSTRUMENTLISTENER_H
#include <juce_audio_basics/juce_audio_basics.h>
#include "MPENoteEvent.h"
#include "DAWTransportData.h"

class MyMPEInstrumentListener: public juce::MPEInstrument::Listener {
public:

    explicit MyMPEInstrumentListener(DAWTransportData* transportData);
    MyMPEInstrumentListener() = delete;

    void noteAdded(juce::MPENote newNote) override;

    ~MyMPEInstrumentListener() override = default;

    void notePressureChanged(juce::MPENote changedNote) override;

    void notePitchbendChanged(juce::MPENote changedNote) override;

    void noteTimbreChanged(juce::MPENote changedNote) override;

    void noteKeyStateChanged(juce::MPENote changedNote) override;

    void zoneLayoutChanged() override;

    void noteReleased(juce::MPENote finishedNote) override;

    std::vector<MPENoteEvent> createNoteEventVector();

    void updateNotes();

    void toggleRecording();
    bool isRecording() const;

    void clearRecordedNotes();

private:
    std::unordered_map<juce::uint16 , MPENoteEvent> unfinishedNotes;
    DAWTransportData* dawTransportData;
    std::vector<MPENoteEvent> noteEventVector{};
    bool recording{false};
    bool clearBeforeRecording{true};
};


#endif //PRACTICEPLAYINGINDAW_MYMPEINSTRUMENTLISTENER_H
