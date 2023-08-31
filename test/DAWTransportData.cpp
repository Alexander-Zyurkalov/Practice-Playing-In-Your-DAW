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

        for (int i = 0; i < numberOfBars; ++i)
        {
            dawTransportData.set(0, 0, barPPQPositions[i]);
            REQUIRE(dawTransportData.getNumBars() == i);
        }

        for (int i = 1; i < numberOfBars; ++i)
        {
            dawTransportData.set(0, 1, barPPQPositions[i]);
            REQUIRE(dawTransportData.getNumBars() == i - 1);
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
        dawTransportData.set(0, 0, barPPQPositions[numberOfBars]);
        for (int i = 0; i < numberOfBars - 1; ++i)
        {
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i]) == barPPQPositions[i + 1]);
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i] + 1) == barPPQPositions[i + 1]);
        }
        dawTransportData.set(0, barPPQPositions[1], barPPQPositions[numberOfBars]);
        for (int i = 1; i < numberOfBars; ++i)
        {
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i]) == barPPQPositions[i + 1]);
            REQUIRE(dawTransportData.getNextBarPpqPosition(barPPQPositions[i] + 1) == barPPQPositions[i + 1]);
        }
    }
}