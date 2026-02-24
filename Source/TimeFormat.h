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
    /**
     * returns the duration in seconds in a suitable audiovisual UI format.
     * Example:
     * durationInSeconds -> 80.0
     * return value -> "1:20"
     *
     */
    static juce::String formatTrackDuration(double durationInSeconds);
    
    /**
     * returns a time value in the form "HH:MM"
     * Example:
     * return value -> "14:46"
     */
    static juce::String formatTimeAsHHMM(const juce::Time& t);
};
