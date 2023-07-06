//
// Created by Alexandr Zyurkalov on 06.07.23.
//

#include "Track.h"

Track::Track()
{
    static int idCounter = 0;
    id = ++idCounter;
    name = "Track " + juce::String (id);
}

int Track::getId() const
{
    return id;
}

juce::String Track::getName() const
{
    return name;
}

void Track::setName (const juce::String& newName)
{
    name = newName;
}