#include "catch2/catch_test_macros.hpp"
#include "../src/processing/MyMPEInstrumentListener.h"


static void playNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                      double step,
                      std::function<double()> shift);

TEST_CASE("Recording", "[MyMPEInstrumentListenerTest]"){
    DAWTransportData dawTransportData{};
    dawTransportData.set(0.0, 0.0, 2.0);
    dawTransportData.setBpm(120.0);


    MyMPEInstrumentListener myMPEInstrumentListener{&dawTransportData};
    myMPEInstrumentListener.toggleRecording();
    REQUIRE(myMPEInstrumentListener.isRecording());
    REQUIRE(myMPEInstrumentListener.isJustStartedRecording());
    myMPEInstrumentListener.updateNotes(0.0);
    REQUIRE(!myMPEInstrumentListener.isJustStartedRecording());

    double step = 0.25;
    playNotes(dawTransportData, myMPEInstrumentListener, step, [](){return 0.0;});

    myMPEInstrumentListener.toggleRecording();
    REQUIRE(!myMPEInstrumentListener.isRecording());

    std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
    REQUIRE(noteEventVector.size() == 8);
    for(size_t i = 0; i < noteEventVector.size(); i++){
        REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
        REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
        REQUIRE(noteEventVector[i].getMpeNote().noteID == 49);
        REQUIRE(noteEventVector[i].getNoteIndex() == i);
        REQUIRE(!noteEventVector[i].thereIsPlayedNote());
        REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
    }

    SECTION("Let's play precisely!")
    {
        REQUIRE(!myMPEInstrumentListener.isRecording());
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return 0.0; });
        noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }


    SECTION("Let's play with a shift")
    {
        REQUIRE(!myMPEInstrumentListener.isRecording());
        constexpr double d = 1 / 16.0;
        playNotes(dawTransportData, myMPEInstrumentListener, step, [d]() { return d; });
        noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].getPlayedNoteStartPositionShift() == d * 8.0f);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1) + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }
}

static void playNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                      double step, std::function<double()> shift)
{
    double position = 0.0;
    juce::MPENote prevNote{};

    do{
        juce::MPENote newNote{};
        newNote.noteID = 49;

        dawTransportData.setPpqPositionNotSynced(position + shift());
        dawTransportData.set(position + shift(), 0.0, 2.0);

        myMPEInstrumentListener.noteReleased(prevNote);
        myMPEInstrumentListener.noteAdded(newNote);

        position += step;

        dawTransportData.setPpqPositionNotSynced(position + shift());
        dawTransportData.set(position+ shift(), 0.0, 2.0);
        prevNote = newNote;
    } while (position < 2.0);

    myMPEInstrumentListener.noteReleased(prevNote);
}
