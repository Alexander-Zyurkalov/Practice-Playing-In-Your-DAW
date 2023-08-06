#include "catch2/catch_test_macros.hpp"
#include "../src/processing/MyMPEInstrumentListener.h"


static void playNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                      double step,
                      std::function<double()> shift,
                      std::function<double(size_t)> noteGenerator = [](size_t i) { return 49 + i; });

static void recordInitialNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                               double step,
                               std::function<double(size_t)> noteGenerator = [](size_t i) { return 49 + i; });


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

    double step = 0.25;


    SECTION("Let's play precisely!")
    {

        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return 0.0; });
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }

    SECTION("Let's play precisely once and after that not to play!")
    {

        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return 0.0; });
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].isPlayed());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
        myMPEInstrumentListener.resetPlayedNotes();

        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return 0.0; }, [](size_t i) { return 21; });
        noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(!noteEventVector[i].isPlayed());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == noteEventVector[i].getPpqReleasePosition());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == noteEventVector[i].getPpqReleasePosition());
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }

    SECTION("Let's record and not to play!")
    {

        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].thereIsPlayedNote());
            REQUIRE(!noteEventVector[i].isPlayed());
        }
        myMPEInstrumentListener.resetPlayedNotes();

        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return 0.0; }, [](size_t i) { return 21; });
        noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(!noteEventVector[i].isPlayed());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == noteEventVector[i].getPpqReleasePosition());
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == noteEventVector[i].getPpqReleasePosition());
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }

    SECTION("Let's play with a shift")
    {
        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        constexpr double d = 1 / 16.0;
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return d; });
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].getPlayedNoteStartPositionShift() == d/MPENoteEvent::MAX_NOTE_SHIFT);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1) + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }

    SECTION("Let's play with a shift but with the only note ID")
    {
        int (*noteGenerator)(size_t) = [](size_t i) { return 49; };
        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step, noteGenerator);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        constexpr double d = 1 / 16.0;
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return d; }, noteGenerator);
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
            REQUIRE(noteEventVector[i].thereIsPlayedNote());
            REQUIRE(noteEventVector[i].getPlayedNoteStartPositionShift() == d/MPENoteEvent::MAX_NOTE_SHIFT);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqStartPosition() == step * i + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getPpqReleasePosition() == step * (i + 1) + d);
            REQUIRE(noteEventVector[i].getPlayedNote().getMpeNote().noteID == 49);
            REQUIRE(noteEventVector[i].getPlayedNote().getNoteIndex() == i);
            REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
        }
    }

    SECTION("Let's play with a too big shift")
    {
        recordInitialNotes(dawTransportData, myMPEInstrumentListener, step);
        REQUIRE(!myMPEInstrumentListener.isRecording());
        constexpr double d = 1 / 8.0 ;
        playNotes(dawTransportData, myMPEInstrumentListener, step, []() { return d; });
        std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
        REQUIRE(noteEventVector.size() == 8);
        for (size_t i = 0; i < noteEventVector.size(); i++)
        {
            REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
            REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
            REQUIRE(noteEventVector[i].getMpeNote().noteID == 49 + i);
            REQUIRE(noteEventVector[i].getNoteIndex() == i);
        }
    }
}

static void playNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                      double step, std::function<double()> shift,
                      std::function<double(size_t)> noteGenerator)
{
    double position = 0.0;
    juce::MPENote prevNote{};
    size_t i = 0;
    do{
        juce::MPENote newNote{};
        newNote.noteID =noteGenerator(i);

        dawTransportData.setPpqPositionNotSynced(position + shift());
        dawTransportData.set(position + shift(), 0.0, 2.0);

        myMPEInstrumentListener.noteReleased(prevNote);
        myMPEInstrumentListener.noteAdded(newNote);

        position += step;
        myMPEInstrumentListener.updateNotes(position + shift());

        dawTransportData.setPpqPositionNotSynced(position + shift());
        dawTransportData.set(position+ shift(), 0.0, 2.0);
        prevNote = newNote;
        i++;
    } while (position < 2.0);
    myMPEInstrumentListener.updateNotes(position + shift());
    myMPEInstrumentListener.noteReleased(prevNote);
}

static void recordInitialNotes(DAWTransportData &dawTransportData, MyMPEInstrumentListener &myMPEInstrumentListener,
                               double step, std::function<double(size_t)> noteGenerator)
{
    playNotes(dawTransportData, myMPEInstrumentListener, step, [](){return 0.0;}, noteGenerator);

    myMPEInstrumentListener.toggleRecording();
    REQUIRE(!myMPEInstrumentListener.isRecording());

    std::vector<MPENoteEvent> noteEventVector = myMPEInstrumentListener.createNoteEventVector();
    REQUIRE(noteEventVector.size() == 8);
    for(size_t i = 0; i < noteEventVector.size(); i++){
        REQUIRE(noteEventVector[i].getPpqStartPosition() == step * i);
        REQUIRE(noteEventVector[i].getPpqReleasePosition() == step * (i + 1));
        REQUIRE(noteEventVector[i].getMpeNote().noteID == noteGenerator(i));
        REQUIRE(noteEventVector[i].getNoteIndex() == i);
        REQUIRE(!noteEventVector[i].getPlayedNote().thereIsPlayedNote());
    }
}
