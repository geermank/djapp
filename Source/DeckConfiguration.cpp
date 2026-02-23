/*
  ==============================================================================

    DeckConfiguration.cpp
    Created: 20 Feb 2026 7:44:45pm
    Author:  German

  ==============================================================================
*/

#include "DeckConfiguration.h"

DeckConfiguration::DeckConfiguration(juce::String name, juce::String trackUrl, double speed, double volume) : name(name), trackUrl(trackUrl), speed(speed), volume(volume)
{
    
}

DeckConfiguration::DeckConfiguration(juce::String name,
                                     juce::String trackUrl,
                                     double speed,
                                     double volume,
                                     std::vector<HotCueButtonInfo> hotCues) :
                                     name(name),
                                     trackUrl(trackUrl),
                                     speed(speed),
                                     volume(volume),
                                     hotCues(hotCues)
{
    
}
