//
// Created by Alexandr Zyurkalov on 31.08.23.
//

#include "DAWTransportData.h"

bool DAWTransportData::changed(double ppq, int num, int denom) const
{
    return getNumerator(ppq) != num || getDenominator(ppq) != denom;
}

void DAWTransportData::setTimeSignature(double ppq, int num, int denom)
{
    timeSignatures[ppq] = {num, denom};
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

double DAWTransportData::getNumBars(double ppq) const {
    double ppqLoopLength = (ppqEndLoopPosition -
            std::max(getTimeSignatureChangePosition(ppq), ppqStartLoopPosition));
    double ppqBerBeat = 4.0 / getDenominator(ppq);
    double ppqPerBar = ppqBerBeat * getNumerator(ppq);
    double numBars = ppqLoopLength / ppqPerBar;
    return numBars;
}

double DAWTransportData::getPpqPosition() const {
    return ppqPosition - ppqStartLoopPosition;
}

void DAWTransportData::setBpm(double bpm_) {
    this->bpm = bpm_;
}

bool DAWTransportData::bpmChanged(double bpm_) const
{
    return this->bpm != bpm_;
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

void DAWTransportData::setPpqPositionNotSynced(double ppqPosition_) {
    ppqPositionNotSynced = ppqPosition_;
    ppqPositionNotSyncedTimeUpdate = std::chrono::high_resolution_clock::now();
}

double DAWTransportData::getNextBarPpqPosition(double ppq) const
{
    double ppqLoopLength = (ppqEndLoopPosition -
                            std::max(getTimeSignatureChangePosition(ppq), ppqStartLoopPosition));
    double numBars = getNumBars(ppq);
    double ppqPerBar = ppqLoopLength / numBars;
    double ppqPositionInLoop = ppq - getTimeSignatureChangePosition(ppq);
    double ppqPositionInBar = std::fmod(ppqPositionInLoop, ppqPerBar);
    double ppqPositionInNextBar = ppqPerBar - ppqPositionInBar;
    double nextBarPpqPosition = ppq + ppqPositionInNextBar;
    if (nextBarPpqPosition > getNextTimeSignatureChangePosition(ppq))
        nextBarPpqPosition = getNextTimeSignatureChangePosition(ppq);
    return nextBarPpqPosition;
}

double DAWTransportData::getNextBeatPpqPosition(double ppq) const {
    double ppqBerBeat = 4.0 / getDenominator(ppq);
    double ppqPerBar = ppqBerBeat * getNumerator(ppq);
    double ppqPositionInLoop = ppq - getTimeSignatureChangePosition(ppq);
    double ppqPositionInBar = std::fmod(ppqPositionInLoop, ppqPerBar);
    double ppqPositionInNextBeat = ppqBerBeat - std::fmod(ppqPositionInBar, ppqBerBeat);
    double nextBeatPpqPosition = ppq + ppqPositionInNextBeat;
    return nextBeatPpqPosition;
}

bool DAWTransportData::isBarBorder(double ppq) const {
    return ppq == getPpqStartLoopPosition() || getTimeSignatureChangePosition(ppq) == ppq ||
            getNextBarPpqPosition(ppq-0.000001) == ppq;
}

double DAWTransportData::getTimeSignatureChangePosition(double ppq) const
{
    auto it = timeSignatures.upper_bound(ppq);
    if (it != timeSignatures.begin())
        --it;
    if (it == timeSignatures.begin() || it == timeSignatures.end())
        return 0;
    return it->first;
}

int DAWTransportData::getNumerator(double ppq) const
{
    double position = getTimeSignatureChangePosition(ppq);
    return timeSignatures.at(position).numerator;
}

int DAWTransportData::getDenominator(double ppq) const
{
    double position = getTimeSignatureChangePosition(ppq);
    return timeSignatures.at(position).denominator;
}

double DAWTransportData::getNextTimeSignatureChangePosition(double ppq) const
{
    auto it = timeSignatures.upper_bound(ppq);
    if (it == timeSignatures.end())
        return std::numeric_limits<double>::max();
    return it->first;
}
