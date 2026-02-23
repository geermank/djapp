#pragma once

#include <JuceHeader.h>

/**
 * Simple abstraction of the Timer class to perform autosave actions
 */
class AutoSave : public Timer {
    
public:
    AutoSave();
    void timerCallback() override;
    
    virtual void saveProject() = 0;
    
private:
    // auto-save the project every minute
    int delay = 1000 * 60;
    bool saveInProgress = false;
};
