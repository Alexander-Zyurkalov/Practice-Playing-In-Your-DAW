//
// Created by Alexandr Zyurkalov on 31.08.23.
//

#include "catch2/catch_test_macros.hpp"
#include "../src/processing/DAWTransportData.h"

TEST_CASE("numberOfBars", "[DAWTransportData]")
{

    std::vector<std::pair<DAWTransportData,std::vector<double>>> measures{
            {DAWTransportData{4, 4}, {0, 4, 8, 12, 16, 20, 24, 28, 32}},
            {DAWTransportData{3, 4}, {0, 3, 6, 9, 12, 15, 18, 21, 24}},
            {DAWTransportData{5, 4}, {0, 5, 10, 15, 20, 25, 30, 35, 40}},
            {DAWTransportData{3, 8}, {0, 1.5, 3, 4.5, 6, 7.5, 9, 10.5, 12}},
    };

    for (auto& measure : measures)
    {
        DAWTransportData& dawTransportData = measure.first;
        std::vector<double>& barPPQPositions = measure.second;
        int numberOfBars = static_cast<int>(barPPQPositions.size()) - 1;

        for (int i = 1; i < numberOfBars; ++i)
        {
            dawTransportData.setLoop(0.0, 0.0, barPPQPositions[i]);
            REQUIRE(dawTransportData.getNumBars(barPPQPositions[i]) == i);
        }

        for (int i = 1; i < numberOfBars; ++i)
        {
            dawTransportData.setLoop(0.0, 1.0, barPPQPositions[i]);
            REQUIRE(dawTransportData.getNumBars(1.0) == i - 1);
        }
    }
}

TEST_CASE("getNextBarPpqPosition", "[DAWTransportData]")
{
    std::vector<std::pair<DAWTransportData,std::vector<double>>> measures{
            {DAWTransportData{4, 4}, {0, 4, 8, 12, 16, 20, 24, 28, 32}},
            {DAWTransportData{3, 4}, {0, 3, 6, 9, 12, 15, 18, 21, 24}},
            {DAWTransportData{5, 4}, {0, 5, 10, 15, 20, 25, 30, 35, 40}},
            {DAWTransportData{3, 8}, {0, 1.5, 3, 4.5, 6, 7.5, 9, 10.5, 12}},
    };

    for (auto& measure : measures)
    {
        DAWTransportData &dawTransportData = measure.first;
        std::vector<double> &barPPQPositions = measure.second;
        int numberOfBars = static_cast<int>(barPPQPositions.size()) - 1;
        dawTransportData.setLoop(0.0, 0.0, barPPQPositions[numberOfBars]);
        for (int i = 0; i < numberOfBars; ++i)
        {
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i]) == barPPQPositions[i + 1]);
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i] + 1) == barPPQPositions[i + 1]);
        }

        dawTransportData.setLoop(0, barPPQPositions[1], barPPQPositions[numberOfBars]);
        for (int i = 1; i < numberOfBars; ++i)
        {
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i]) == barPPQPositions[i + 1]);
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i] + 1) == barPPQPositions[i + 1]);
        }
    }
}

TEST_CASE("getNextBeatPpqPosition", "[DAWTransportData]")
{
    std::vector<std::pair<DAWTransportData, std::vector<double>>> measures{
            {DAWTransportData{2, 4}, {0, 1, 2, 3, 4, 5, 6, 7, 8}},
            {DAWTransportData{2, 8}, {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4}},
            {DAWTransportData{3, 2}, {0, 2, 4, 6, 8, 10, 12, 14, 16}},
            {DAWTransportData{2, 16}, {0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2}},
            {DAWTransportData{6, 8}, {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5,
                                      5.5, 6, 6.5, 7, 7.5, 8, 8.5}},
    };
    int numberOfBars = 2;
    for (auto& measure : measures)
    {
        DAWTransportData &dawTransportData = measure.first;
        std::vector<double> &beatPpqPositions = measure.second;
        int numberOfBeats = numberOfBars * dawTransportData.getNumerator(0);
        dawTransportData.setLoop(0.0, 0.0, beatPpqPositions[numberOfBeats]);
        for (int i = 0; i <  numberOfBeats - 1; ++i)
        {
            REQUIRE(dawTransportData.getNextBeatPpqPosition(beatPpqPositions[i]) == beatPpqPositions[i + 1]);
            REQUIRE(dawTransportData.isBarBorder(beatPpqPositions[i]) == (i % dawTransportData.getNumerator(beatPpqPositions[i]) == 0));
        }
    }

}

struct TimeSignaturesTestRecord
{

    DAWTransportData dawTransportData;
    std::vector<std::pair<double, TimeSignature>> timeSignatureChanges;
    std::vector<double> beats;
    std::vector<double> barsForTheInitialTimeSignature;
    std::vector<double> bars;
};

TEST_CASE("timeSignatures", "[DAWTransportData]")
{
    std::vector<TimeSignaturesTestRecord> timeSignatureTestRecords{
            {
                    .dawTransportData{4,4},
                    .timeSignatureChanges{
                            {8.0, TimeSignature{4, 8}},
                    },
                    .beats{1, 2, 3, 4, 5, 6, 7,
                           8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16},
                    .barsForTheInitialTimeSignature{0, 4, 8, 12, 16},
                    .bars{4, 8, 10, 12, 14, 16}
            }
    };

    for (TimeSignaturesTestRecord& timeSignatureTestRecord: timeSignatureTestRecords)
    {
        // setting the initial loop
        timeSignatureTestRecord.dawTransportData.setLoop(0.0,
                                                         timeSignatureTestRecord.barsForTheInitialTimeSignature.front(),
                                                         timeSignatureTestRecord.barsForTheInitialTimeSignature.back());

        // checking the initial bars
        size_t numberOfBars = timeSignatureTestRecord.barsForTheInitialTimeSignature.size();
        for (size_t i = 0; i < numberOfBars-1; ++i)
        {
            REQUIRE(timeSignatureTestRecord.dawTransportData.getNextBarPpqPosition(
                    timeSignatureTestRecord.barsForTheInitialTimeSignature[i]) ==
                            timeSignatureTestRecord.barsForTheInitialTimeSignature[i + 1]
            );
        }

        // checking the initial time signature
        double ppq=timeSignatureTestRecord.dawTransportData.getPpqStartLoopPosition();
        while (ppq<timeSignatureTestRecord.dawTransportData.getPpqEndLoopPosition())
        {
            REQUIRE(
                timeSignatureTestRecord.dawTransportData.getTimeSignatureChangePosition(ppq) == 0.0
            );
            ppq+=0.01;
        }

        //setting the new time signatures
        for (std::pair<double, TimeSignature> measureChange: timeSignatureTestRecord.timeSignatureChanges)
        {
            timeSignatureTestRecord.dawTransportData.setTimeSignature(measureChange.first,
                                                                      measureChange.second.numerator,
                                                                      measureChange.second.denominator);
        }

        // checking the beats
        double beat = 0;
        for (double nextBeat: timeSignatureTestRecord.beats)
        {
            REQUIRE(timeSignatureTestRecord.dawTransportData.getNextBeatPpqPosition(beat) == nextBeat);
            beat = nextBeat;
        }

        // checking the bars
        double bar = 0;
        for (double nextBar: timeSignatureTestRecord.bars)
        {
            REQUIRE(timeSignatureTestRecord.dawTransportData.getNextBarPpqPosition(bar) == nextBar);
            bar = nextBar;
        }
    }
}