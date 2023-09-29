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
    std::string testName{};
    DAWTransportData dawTransportData;
    std::vector<std::pair<double, TimeSignature>> timeSignatureChanges;
    std::vector<double> beats;
    std::vector<double> bars;
};

TEST_CASE("timeSignatures", "[DAWTransportData]")
{
    std::vector<TimeSignaturesTestRecord> timeSignatureTestRecords{
            {
                    .testName{"initial time signature"},
                    .dawTransportData{4,4},
                    .timeSignatureChanges{},
                    .beats{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
                    .bars{0, 4, 8, 12, 16, 20, 24}
            },
            {
                    .testName{"Here we don't see any shift in the beats and bars"},
                    .dawTransportData{4,4},
                    .timeSignatureChanges{
                            {8.0, TimeSignature{4, 4}},
                    },
                    .beats{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
                    .bars{0, 4, 8, 12, 16, 20, 24}
            },
            {
                .testName{"Here we should see a shift of 0.5 beats for bars and beats"},
                .dawTransportData{4,4},
                .timeSignatureChanges{
                        {0.5, TimeSignature{4, 4}},
                },
                .beats{0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5},
                .bars{0.5, 4.5, 8.5, 12.5, 16.5, 20.5, 24.5}
            },
            {
                .testName{"We should see shift and new bar positions"},
                .dawTransportData{4,4},
                .timeSignatureChanges{
                        {0.5, TimeSignature{3, 4}},
                },
                .beats{0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5},
                .bars{0.5, 3.5, 6.5, 9.5, 12.5, 15.5, 18.5}
            },
            {
                .testName{"A test for the initial 6/8 time signature"},
                .dawTransportData{6,8},
                .timeSignatureChanges{},
                .beats{0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4},
                .bars{0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36}
            },
            {
                .testName{"A test for the initial 6/8 time signature and further change to 4/4"},
                .dawTransportData{6,8},
                .timeSignatureChanges{{30, {4,4}}},
                .beats{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
                .bars{2, 6, 10, 14, 18, 22, 26, 30, 34, 38}
            },

    };

    for (TimeSignaturesTestRecord& timeSignatureTestRecord: timeSignatureTestRecords)
    {
        SECTION(timeSignatureTestRecord.testName)
        {
            timeSignatureTestRecord.dawTransportData.setLoop(0, 0, 100);

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
}

TEST_CASE("barShift", "[DAWTransportData]")
{
    DAWTransportData dawTransportData{4,4};
    dawTransportData.setLoop(0, 0, 50);
    dawTransportData.setTimeSignature(30, 4, 4);
    REQUIRE(dawTransportData.getBarShift() == 2.0);

}