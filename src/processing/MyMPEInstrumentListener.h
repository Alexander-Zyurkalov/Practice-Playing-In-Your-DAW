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

private:
    std::unordered_map<juce::uint16 , MPENoteEvent> notes;
    DAWTransportData* dawTransportData;
    std::vector<MPENoteEvent> noteEventVector{};
};


#endif //PRACTICEPLAYINGINDAW_MYMPEINSTRUMENTLISTENER_H
