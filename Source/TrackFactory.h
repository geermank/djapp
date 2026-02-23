#pragma once

#include "Track.h"
#include <string>
#include <JuceHeader.h>

class TrackFactory {
public:
    static Track createTrackFromFile(juce::String fileUrl,
                                     juce::AudioFormatManager& formatManager);
    static Track createTrackFromFile(juce::File file,
                                     juce::AudioFormatManager& formatManager);
private:
    static double calculateDurationInSeconds(juce::File& f, juce::AudioFormatManager& formatManager);
};
