/*
  ==============================================================================

    AutoSave.cpp
    Created: 20 Feb 2026 10:46:56pm
    Author:  German

  ==============================================================================
*/

#include "AutoSave.h"
#include "TimeFormat.h"

AutoSave::AutoSave() {
    startTimer(delay);
    nextAutoSaveTime = TimeFormat::formatTimeAsHHMM(nextAutoSaveTimeFromNowMs());
}

void AutoSave::timerCallback() {
    if (saveInProgress) {
        return;
    }
    saveInProgress = true;
    saveProject();
    saveInProgress = false;

    nextAutoSaveTime = TimeFormat::formatTimeAsHHMM(nextAutoSaveTimeFromNowMs());
    onAutoSaveExecuted(TimeFormat::formatTimeAsHHMM(juce::Time::getCurrentTime()),
                       nextAutoSaveTime);
}

juce::Time AutoSave::nextAutoSaveTimeFromNowMs() {
    return juce::Time::getCurrentTime() + juce::RelativeTime::milliseconds(delay);
}

juce::String AutoSave::getNextAutoSaveTime() {
    return nextAutoSaveTime;
}
