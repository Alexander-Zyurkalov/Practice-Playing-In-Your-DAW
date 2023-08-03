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
    do{
        juce::MPENote newNote{};
        newNote.noteID = 49;

        dawTransportData.setPpqPositionNotSynced(position);
        dawTransportData.set(position, 0.0, 2.0);

        myMPEInstrumentListener.noteReleased(prevNote);
        myMPEInstrumentListener.noteAdded(newNote);

        position += 0.25;
        prevNote = newNote;
    } while (position < 2.0);

    position += 0.25;
    dawTransportData.setPpqPositionNotSynced(position);
    dawTransportData.set(position, 0.0, 2.0);
    myMPEInstrumentListener.noteReleased(prevNote);

    myMPEInstrumentListener.toggleRecording();
    REQUIRE(!myMPEInstrumentListener.isRecording());

    std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
    REQUIRE(noteEventVector.size() == 8);
    REQUIRE(noteEventVector[0].getPpqStartPosition() == 0.0);
    REQUIRE(noteEventVector[0].getPpqReleasePosition() == 0.25);
    REQUIRE(noteEventVector[0].getMpeNote().noteID == 49);
    REQUIRE(noteEventVector[0].getNoteIndex() == 0);
//    REQUIRE(!noteEventVector[0].thereIsPlayedNote());





}