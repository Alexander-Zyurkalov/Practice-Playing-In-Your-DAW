#include "catch2/catch_test_macros.hpp"
#include "../src/processing/MyMPEInstrumentListener.h"
TEST_CASE("Recording", "[MyMPEInstrumentListenerTest]")
{
    DAWTransportData dawTransportData{};
    dawTransportData.set(0.0, 0.0, 2.0);
    dawTransportData.setBpm(120.0);


    MyMPEInstrumentListener myMPEInstrumentListener{&dawTransportData};
    myMPEInstrumentListener.toggleRecording();
    REQUIRE(myMPEInstrumentListener.isRecording());
    REQUIRE(myMPEInstrumentListener.isJustStartedRecording());
    myMPEInstrumentListener.updateNotes(0.0);
    REQUIRE(!myMPEInstrumentListener.isJustStartedRecording());

    double position = 0.0;
    juce::MPENote prevNote{};
    double step = 0.25;
    do{
        juce::MPENote newNote{};
        newNote.noteID = 49;

        dawTransportData.setPpqPositionNotSynced(position);
        dawTransportData.set(position, 0.0, 2.0);

        myMPEInstrumentListener.noteReleased(prevNote);
        myMPEInstrumentListener.noteAdded(newNote);

        position += step;

        dawTransportData.setPpqPositionNotSynced(position);
        dawTransportData.set(position, 0.0, 2.0);
        prevNote = newNote;
    } while (position < 2.0);

    myMPEInstrumentListener.noteReleased(prevNote);

    myMPEInstrumentListener.toggleRecording();
    REQUIRE(!myMPEInstrumentListener.isRecording());

    std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
    REQUIRE(noteEventVector.size() == 8);
    for(size_t i = 0; i < noteEventVector.size(); i++){
        REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
        REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
        REQUIRE(noteEventVector[i].getMpeNote().noteID == 49);
        REQUIRE(noteEventVector[i].getNoteIndex() == i);
    }
    
//    REQUIRE(!noteEventVector[0].thereIsPlayedNote());





}