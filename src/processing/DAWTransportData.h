//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
#define PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <chrono>

class
DAWTransportData
{
public:
    explicit DAWTransportData(int num=4, int denom=4) :
            numerator{num}, denominator{denom}
    {}

    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
    double getTimeSignatureChangePosition() const;
    bool changed(int num, int denom) const;

    void setTimeSignature(double ppq, int num, int denom);


    bool changed(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos) const;

    void setLoop(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos);

    double getNumBars() const;

    double getPpqPosition() const;

    double getPpqStartLoopPosition() const ;

    double getPpqEndLoopPosition() const;

    double getPpqPositionNotSynced() const;

    void setPpqPositionNotSynced(double ppqPosition);

    bool bpmChanged(double bpm) const;

    void setBpm(double bpm);

    double getBarShift() const;
private:
    int numerator;
    int denominator;
    double timeSignatureChange = 0;
    double ppqPosition = 0.0;
    double ppqStartLoopPosition = 0.0;
    double ppqEndLoopPosition = 16.0;
    double ppqPositionNotSynced = 0.0;
    double bpm = 120.0;
    std::chrono::high_resolution_clock::time_point ppqPositionNotSyncedTimeUpdate{std::chrono::high_resolution_clock::now()} ;

};

#endif //PRACTICEPLAYINGINDAW_DAWTRANSPORTDATA_H
