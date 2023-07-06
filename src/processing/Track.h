//
// Created by Alexandr Zyurkalov on 06.07.23.
//

#ifndef PRACTICEPLAYINGINDAW_TRACK_H
#define PRACTICEPLAYINGINDAW_TRACK_H

#include "juce_core/juce_core.h"

class Track {
public:
    Track();
    int getId() const;
    juce::String getName() const;
    void setName (const juce::String& newName);
private:
    int id;
    juce::String name;
};


#endif //PRACTICEPLAYINGINDAW_TRACK_H
