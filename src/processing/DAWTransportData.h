//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
#define PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

struct Measure
{
    int numerator;
    int denominator;
};

class DAWTransportData
{
public:
    explicit DAWTransportData(int num=4, int denom=4) : measure{num, denom}
    {}

    int getNumerator() const { return measure.numerator; }
    int getDenominator() const { return measure.denominator; }
    bool changed(double ppq, int num, int denom) const;

    void set(double ppq, int num, int denom);


    bool changed(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos) const;

    void set(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos);

    int getNumBars() const;

    double getPpqPosition() const;

    double getPpqStartLoopPosition() const ;

    double getPpqEndLoopPosition() const;

    double getPpqPositionNotSynced() const;

    void setPpqPositionNotSynced(double ppqPosition);

    bool bpmChanged(double bpm_) const;

    void setBpm(double bpm_);

    double getNextBarPpqPosition(double ppq ) const;
    double getNextBeatPpqPosition(double ppq ) const;
    bool isBarBorder(double ppq) const;

private:
    Measure measure;
    double ppqPosition = 0.0;
    double ppqStartLoopPosition = 0.0;
    double ppqEndLoopPosition = 16.0;
    double ppqPositionNotSynced = 0.0;
    double bpm = 120.0;
    std::map<double, Measure> measures;
    std::chrono::high_resolution_clock::time_point ppqPositionNotSyncedTimeUpdate{std::chrono::high_resolution_clock::now()} ;

    double getBPMStartPpqPosition(double ppq) const;
};

#endif //PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
