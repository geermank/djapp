#pragma once

#include <JuceHeader.h>
#include "Track.h"

class TrackParser {

public:
    static juce::String serialize(std::vector<Track> tracks);
    static std::vector<Track> deserialize(juce::String json);

private:
    static juce::var toVar(Track track);
    static Track fromVar(juce::var& v);
};
