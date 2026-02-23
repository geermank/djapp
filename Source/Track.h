/*
  ==============================================================================

    Track.h
    Created: 16 Feb 2026 7:03:03pm
    Author:  German

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Track {
    
public:
    Track(juce::String name,
          juce::String url,
          juce::String duration);
    juce::String name;
    juce::String url;
    juce::String duration;
};
