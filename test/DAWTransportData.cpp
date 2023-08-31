//
// Created by Alexandr Zyurkalov on 31.08.23.
//

#include "catch2/catch_test_macros.hpp"
#include "../src/processing/DAWTransportData.h"

TEST_CASE("numberOfBars", "[DAWTransportData]")
{

    DAWTransportData dawTransportData{4, 4};
    std::vector<double> barPPQPositions{0, 4, 8, 12, 16, 20, 24, 28, 32};
    int numberOfBars = 8;
    for (int i = 0; i < numberOfBars; ++i)
    {
        dawTransportData.set(0, 0, barPPQPositions[i]);
        REQUIRE(dawTransportData.getNumBars() == i);
    }

    for (int i = 1; i < numberOfBars; ++i)
    {
        dawTransportData.set(0, 1, barPPQPositions[i]);
        REQUIRE(dawTransportData.getNumBars() == i-1);
    }

}