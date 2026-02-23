/*
  ==============================================================================

    TimeFormat.h
    Created: 16 Feb 2026 8:20:36pm
    Author:  German

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TimeFormat {
public:
    static juce::String formatTrackDuration(double durationInSeconds);
};
