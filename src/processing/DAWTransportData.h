//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
#define PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H

#include <juce_audio_processors/juce_audio_processors.h>

class DAWTransportData
{
public:
    explicit DAWTransportData(int num=4, int denom=4) :
            numerator{num}, denominator{denom}
    {}

    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
    bool changed(int num, int denom) const
    {
        return this->numerator != num || this->denominator != denom;
    }
    void set(int num, int denom)
    {
        this->numerator = num;
        this->denominator = denom;
    }

    bool changed(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos) const
    {
        return this->ppqPosition != ppqPos ||
               this->ppqStartLoopPosition != ppqStartLoopPos ||
               this->ppqEndLoopPosition != ppqEndLoopPos;
    }

    void set(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos)
    {
        this->ppqPosition = ppqPos;
        this->ppqStartLoopPosition = ppqStartLoopPos;
        this->ppqEndLoopPosition = ppqEndLoopPos;
    }

    int getNumBars() const {
        double ppqLoopLength = (ppqEndLoopPosition - ppqStartLoopPosition);
        double quarterNotesPerBeat = 4.0 / denominator;
        double ppqPerBeat = quarterNotesPerBeat * numerator;
        double numBars = ppqLoopLength / ppqPerBeat;
        return static_cast<int>(numBars);
    }

    double getPpqPosition() const {
        return ppqPosition;
    }

    double getPpqStartLoopPosition() const {
        return ppqStartLoopPosition;
    }

    double getPpqEndLoopPosition() const {
        return ppqEndLoopPosition;
    }

private:
    int numerator;
    int denominator;
    double ppqPosition = 0.0;
    double ppqStartLoopPosition = 0.0;
    double ppqEndLoopPosition = 16.0;


    //TODO: we also need to know whether the transport is playing or not
};

#endif //PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
