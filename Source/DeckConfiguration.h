#pragma once

#include <JuceHeader.h>
#include "HotCueButtonInfo.h"

class DeckConfiguration {
    
public:
    DeckConfiguration(juce::String name, 
                      juce::String trackUrl);
    
    DeckConfiguration(juce::String name,
                      juce::String trackUrl,
                      double speed,
                      double volume,
                      double lowEq,
                      double midEq,
                      double highEq,
                      std::vector<HotCueButtonInfo> hotCues);
    
    juce::String name;
    juce::String trackUrl;
    double speed;
    double volume;
    double lowEq;
    double midEq;
    double highEq;
    std::vector<HotCueButtonInfo> hotCues;
};
