#pragma once

#include <JuceHeader.h>
#include "Track.h"

class TrackParser {

public:
    /**
     * Transforms the given array of tracks into a String representation of it, which can be later used
     * to deserialized and restore it
     */
    static juce::String serialize(std::vector<Track> tracks);
    
    /**
     * Receives a serialized array of tracks and returns an array of track instances
     */
    static std::vector<Track> deserialize(juce::String json);
private:
    static juce::var toVar(Track track);
    static Track fromVar(juce::var& v);
};
