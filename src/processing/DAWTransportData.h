//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
#define PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

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
        return ppqPosition - ppqStartLoopPosition;
    }

    double getPpqStartLoopPosition() const {
        return ppqStartLoopPosition;
    }

    double getPpqEndLoopPosition() const {
        return ppqEndLoopPosition;
    }

    double getPpqPositionNotSynced() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::ratio<60, 1>> diff = now - ppqPositionNotSyncedTimeUpdate;

        double ppq = 0 ; // bpm * diff.count();
        return ppqPositionNotSynced + ppq;
    }

    void setPpqPositionNotSynced(double ppqPosition) {
        ppqPositionNotSynced = ppqPosition;
        ppqPositionNotSyncedTimeUpdate = std::chrono::high_resolution_clock::now();
    }

    bool bpmChanged(double bpm) const
    {
        return this->bpm != bpm;
    }

    void setBpm(double bpm)
    {
        this->bpm = bpm;
    }
private:
    int numerator;
    int denominator;
    double ppqPosition = 0.0;
    double ppqStartLoopPosition = 0.0;
    double ppqEndLoopPosition = 16.0;
    double ppqPositionNotSynced = 0.0;
    double bpm = 120.0;
    std::chrono::high_resolution_clock::time_point ppqPositionNotSyncedTimeUpdate{std::chrono::high_resolution_clock::now()} ;

};

#endif //PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
