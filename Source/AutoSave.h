#pragma once

#include <JuceHeader.h>

/**
 * Simple abstraction of the Timer class to perform autosave actions
 */
class AutoSave : public Timer {
    
public:
    AutoSave();
    void timerCallback() override;
    
    /**
     * Requests to save the project after the timeout has triggered
     */
    virtual void saveProject() = 0;
    
    /**
     * It calculates the next time autosave will be triggered (nextSaveTime)
     * and also sends the last time the project was saved (lastSaveTime), both formatted as HH:MM
     */
    virtual void onAutoSaveExecuted(juce::String lastSaveTime, juce::String nextSaveTime) = 0;
    
    juce::String getNextAutoSaveTime();
private:
    // auto-save the project every three minutes
    int delay = (1000 * 60) * 5;
    bool saveInProgress = false;
    
    juce::String nextAutoSaveTime;
    
    juce::Time nextAutoSaveTimeFromNowMs();
};
