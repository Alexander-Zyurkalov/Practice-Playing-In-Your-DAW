//
// Created by Alexandr Zyurkalov on 04.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_TIMESIGNATURE_H
#define PRACTICEPLAYINGINDAW_TIMESIGNATURE_H

#include <juce_audio_processors/juce_audio_processors.h>

class TimeSignature
{
public:
    explicit TimeSignature(int num=4, int denom=4) :
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
private:
    int numerator;
    int denominator;
};

#endif //PRACTICEPLAYINGINDAW_TIMESIGNATURE_H
