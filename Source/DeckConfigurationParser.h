#pragma once

#include <JuceHeader.h>
#include "DeckConfiguration.h"

class DeckConfigurationParser {
    
public:
    static juce::String serialize(std::vector<DeckConfiguration> configs);
    static std::vector<DeckConfiguration> deserialize(juce::String json);

private:
    static juce::var toVar(DeckConfiguration config);
    static DeckConfiguration fromVar(const juce::var& v);
    static juce::var toVar(HotCueButtonInfo hotCue);
    static HotCueButtonInfo fromVarToHotCue(const juce::var& v);
};
