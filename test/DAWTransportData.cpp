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
            REQUIRE(dawTransportData.getNumBars() == i);
        }

    }
}
struct TimeSignature
{
    int numerator;
    int denominator;
};
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
                    .timeSignatureChanges{},
                    .beats{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
                    .barsForTheInitialTimeSignature{0, 4, 8, 12, 16, 20, 24},
                    .bars{0, 4, 8, 12, 16, 20, 24}
            },
//            {
//                    .dawTransportData{4,4},
//                    .timeSignatureChanges{
//                            {8.0, TimeSignature{4, 8}},
//                    },
//                    .beats{0, 1, 2, 3, 4, 5, 6, 7,
//                           8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15, 15.5, 16},
//                    .barsForTheInitialTimeSignature{0, 4, 8, 12, 16},
//                    .bars{0, 4, 8, 10, 12, 14, 16}
//            },
//            {
//                    .dawTransportData{4,4},
//                    .timeSignatureChanges{
//                            {7.0, TimeSignature{4, 8}},
//                    },
//                    .beats{0, 1, 2, 3, 4, 5, 6, 7,
//                           7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14, 14.5, 15},
//                    .barsForTheInitialTimeSignature{0, 4, 8, 12, 16},
//                    .bars{0, 4, 7, 9, 11, 13, 15}
//            },
    };

    for (TimeSignaturesTestRecord& timeSignatureTestRecord: timeSignatureTestRecords)
    {
        // setting the initial loop
        timeSignatureTestRecord.dawTransportData.setLoop(0.0,
                                                         timeSignatureTestRecord.barsForTheInitialTimeSignature.front(),
                                                         timeSignatureTestRecord.barsForTheInitialTimeSignature.back());

        // checking the initial bars
        size_t numberOfBars = timeSignatureTestRecord.barsForTheInitialTimeSignature.size();
        for (size_t i = 0; i < numberOfBars; ++i)
        {
            REQUIRE(
                timeSignatureTestRecord.dawTransportData.getBarPosition(i) ==
                    timeSignatureTestRecord.barsForTheInitialTimeSignature[i]
            );
        }

        // checking the initial time signature
        double ppq=timeSignatureTestRecord.dawTransportData.getPpqStartLoopPosition();
        while (ppq<timeSignatureTestRecord.dawTransportData.getPpqEndLoopPosition())
        {
            REQUIRE(
                timeSignatureTestRecord.dawTransportData.getTimeSignatureChangePosition() ==
                    timeSignatureTestRecord.dawTransportData.getPpqStartLoopPosition()
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
        double beatNum = 0.0;
        for (double beat: timeSignatureTestRecord.beats)
        {
            REQUIRE(timeSignatureTestRecord.dawTransportData.getBeatPosition(beatNum) == beat);
            beatNum++;
        }

        // checking the bars
        double barNum = 0.0;
        for (double bar: timeSignatureTestRecord.bars)
        {
            REQUIRE(timeSignatureTestRecord.dawTransportData.getBarPosition(barNum) == bar);
            barNum++;
        }
    }
}