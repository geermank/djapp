/*
  ==============================================================================

    TimeFormat.cpp
    Created: 16 Feb 2026 8:20:36pm
    Author:  German

  ==============================================================================
*/

#include "TimeFormat.h"

juce::String TimeFormat::formatTrackDuration(double durationInSeconds) {
    int intDurationInSeconds = static_cast<int>(durationInSeconds);
    long hours = intDurationInSeconds / 3600;
    long minutes = (intDurationInSeconds % 3600) / 60;
    long seconds = intDurationInSeconds % 60;

    if (hours > 0)
        // formatted methods docs: https://docs.juce.com/develop/classjuce_1_1String.html#a8480cd0b4b284059b5b3fdd7a057f3a9
        // show hours only when the duration is longer than an hour
        return juce::String::formatted("%02ld:%02ld:%02ld", hours, minutes, seconds);
    else
        return juce::String::formatted("%02ld:%02ld", minutes, seconds);
}

juce::String TimeFormat::formatTimeAsHHMM(const juce::Time& t)
{
    // %H = hours 00-23, %M = minutes 00-59
    return t.formatted("%H:%M");
}
