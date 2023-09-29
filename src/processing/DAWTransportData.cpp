//
// Created by Alexandr Zyurkalov on 31.08.23.
//

#include "DAWTransportData.h"

bool DAWTransportData::changed(int num, int denom) const
{
    return this->numerator != num || this->denominator != denom;
}

void DAWTransportData::setTimeSignature(double ppq, int num, int denom)
{
    this->numerator = num;
    this->denominator = denom;
    this->timeSignatureChange = ppq;
}

bool DAWTransportData::changed(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos) const
{
    return this->ppqPosition != ppqPos ||
           this->ppqStartLoopPosition != ppqStartLoopPos ||
           this->ppqEndLoopPosition != ppqEndLoopPos;
}

void DAWTransportData::setLoop(double ppqPos, double ppqStartLoopPos, double ppqEndLoopPos)
{
    this->ppqPosition = ppqPos;
    this->ppqStartLoopPosition = ppqStartLoopPos;
    this->ppqEndLoopPosition = ppqEndLoopPos;
}

double DAWTransportData::getNumBars() const {
    double ppqLoopLength = (ppqEndLoopPosition - ppqStartLoopPosition);
    double quarterNotesPerBeat = 4.0 / denominator;
    double ppqPerBar = quarterNotesPerBeat * numerator;
    double numBars = ppqLoopLength / ppqPerBar;
    return static_cast<int>(numBars);
}

double DAWTransportData::getPpqPosition() const {
    return ppqPosition - ppqStartLoopPosition;
}

void DAWTransportData::setBpm(double bpm) {
    this->bpm = bpm;
}

bool DAWTransportData::bpmChanged(double bpm) const
{
    return this->bpm != bpm;
}


double DAWTransportData::getPpqPositionNotSynced() const {
//    auto now = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double, std::ratio<60, 1>> diff = now - ppqPositionNotSyncedTimeUpdate;

    double ppq = 0; //bpm * diff.count();
    return ppqPositionNotSynced + ppq;
}

double DAWTransportData::getPpqEndLoopPosition() const {
    return ppqEndLoopPosition;
}

double DAWTransportData::getPpqStartLoopPosition() const {
    return ppqStartLoopPosition;
}

void DAWTransportData::setPpqPositionNotSynced(double ppqPosition) {
    ppqPositionNotSynced = ppqPosition;
    ppqPositionNotSyncedTimeUpdate = std::chrono::high_resolution_clock::now();
}

double DAWTransportData::getTimeSignatureChangePosition() const {
    return timeSignatureChange;
}

double DAWTransportData::getBarShift() const
{
    double quarterNotesPerBeat = 4.0 / denominator;
    double ppqPerBar = quarterNotesPerBeat * numerator;
    double rest = std::fmod(getTimeSignatureChangePosition(), ppqPerBar);
    return rest;
}

double DAWTransportData::getBarPosition(double barNumber) const
{
    double quarterNotesPerBeat = 4.0 / denominator;
    double ppqPerBar = quarterNotesPerBeat * numerator;
    return barNumber * ppqPerBar + getBarShift();
}

double DAWTransportData::getBeatPosition(double beatNumber) const
{
    double quarterNotesPerBeat = 4.0 / denominator;
    double ppqPerBeat = quarterNotesPerBeat;
    return beatNumber * ppqPerBeat + getBarShift();
}