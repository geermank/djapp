/*
  ==============================================================================

    DeckConfiguration.cpp
    Created: 20 Feb 2026 7:44:45pm
    Author:  German

  ==============================================================================
*/

#include "DeckConfiguration.h"

DeckConfiguration::DeckConfiguration(
                                     juce::String name,
                                     juce::String trackUrl) : name(name),
                                            trackUrl(trackUrl),
                                            speed(1.0),
                                            volume(1.0),
                                            lowEq(0.0),
                                            midEq(0.0),
                                            highEq(0.0)
{
    
}

DeckConfiguration::DeckConfiguration(juce::String name,
                                     juce::String trackUrl,
                                     double speed,
                                     double volume,
                                     double lowEq,
                                     double midEq,
                                     double highEq,
                                     std::vector<HotCueButtonInfo> hotCues) :
                                     name(name),
                                     trackUrl(trackUrl),
                                     speed(speed),
                                     volume(volume),
                                     lowEq(lowEq),
                                     midEq(midEq),
                                     highEq(highEq),
                                     hotCues(hotCues)
{
    
}
