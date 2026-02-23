#pragma once

#include <JuceHeader.h>
#include "HotCueButtonInfo.h"

class DeckConfiguration {
    
public:
    DeckConfiguration(juce::String name, 
                      juce::String trackUrl,
                      double speed,
                      double volume);
    
    DeckConfiguration(juce::String name,
                      juce::String trackUrl,
                      double speed,
                      double volume,
                      std::vector<HotCueButtonInfo> hotCues);
    
    juce::String name;
    juce::String trackUrl;
    double speed;
    double volume;
    std::vector<HotCueButtonInfo> hotCues;
};
