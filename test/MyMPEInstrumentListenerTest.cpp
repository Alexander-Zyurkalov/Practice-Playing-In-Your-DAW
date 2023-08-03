#include "catch2/catch_test_macros.hpp"
#include "../src/processing/MyMPEInstrumentListener.h"
TEST_CASE("MyMPEInstrumentListenerTest", "[MyMPEInstrumentListenerTest]")
{
    DAWTransportData dawTransportData{};
    dawTransportData.set(0.0, 0.0, 2.0);
    dawTransportData.setBpm(120.0);


    MyMPEInstrumentListener myMPEInstrumentListener{&dawTransportData};
    myMPEInstrumentListener.toggleRecording();
    REQUIRE(myMPEInstrumentListener.isRecording());

    double position = 0.0;
    juce::MPENote prevNote{};
    do{
        juce::MPENote newNote{};
        newNote.noteID = 49;

        dawTransportData.set(position, 0.0, 2.0);
        myMPEInstrumentListener.noteAdded(newNote);
        myMPEInstrumentListener.noteReleased(prevNote);

//        myMPEInstrumentListener.updateNotes(position);
        position += 0.25;
        prevNote = newNote;
    } while (position < 2.0);

    myMPEInstrumentListener.toggleRecording();
    REQUIRE(!myMPEInstrumentListener.isRecording());



}