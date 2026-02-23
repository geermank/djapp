/*
  ==============================================================================

    HotCueButton.cpp
    Created: 22 Feb 2026 4:50:28pm
    Author:  German

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HotCueButton.h"
#include "TimeFormat.h"

//==============================================================================
HotCueButton::HotCueButton()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

HotCueButton::~HotCueButton()
{
}

void HotCueButton::clearPosition() {
    this->positionInSeconds = -1.0;
    setButtonText("");
}

void HotCueButton::setPositionInSeconds(double positionInSeconds) {
    this->positionInSeconds = positionInSeconds;

    setButtonText(TimeFormat::formatTrackDuration(positionInSeconds));
}

void HotCueButton::setSelected(bool val) {
    selected = val;
    if (selected) {
        setColour(juce::TextButton::buttonColourId, juce::Colours::greenyellow);
    } else {
        removeColour(juce::TextButton::buttonColourId);
    }
}

bool HotCueButton::isSelected() {
    return selected;
}

double HotCueButton::getPositionInSeconds() {
    return positionInSeconds;
}
