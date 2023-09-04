//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
#define PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

struct TimeSignature
{
    int numerator;
    int denominator;
};

class DAWTransportData
{
public:
    explicit DAWTransportData(int num=4, int denom=4) : timeSignatures{{0.0, {num, denom}}}
    {}

    int getNumerator(double ppq) const;
    int getDenominator(double ppq) const;
    bool changed(double ppq, int num, int denom) const;

    void setTimeSignature(double ppq, int num, int denom);


    bool changed(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos) const;

    void setLoop(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos);

    int getNumBars(double ppq) const;

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
    double ppqPosition = 0.0;
    double ppqStartLoopPosition = 0.0;
    double ppqEndLoopPosition = 16.0;
    double ppqPositionNotSynced = 0.0;
    double bpm = 120.0;
    std::map<double, TimeSignature> timeSignatures;
    std::chrono::high_resolution_clock::time_point ppqPositionNotSyncedTimeUpdate{std::chrono::high_resolution_clock::now()} ;

    double getTimeSignatureChangeStartPpqPosition(double ppq) const;
};

#endif //PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
