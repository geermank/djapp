#pragma once

#include <JuceHeader.h>

class HotCueButton  : public juce::TextButton
{
public:
    HotCueButton();
    ~HotCueButton() override;

    /**
     * Changes the selection state of this hot cue button, based on the given value
     */
    void setSelected(bool val);
    
    /**
     * returns true when the button is selected, false otherwise
     */
    bool isSelected();
    
    /**
     * Forgets the stored position in seconds of the track.
     * To do so, it sets an invalid value (-1.0)
     */
    void clearPosition();
    
    /**
     * Stores a position in seconds of a track. It must be bigger or equal to zero
     */
    void setPositionInSeconds(double seconds);
    
    /**
     * returns the stored position in seconds of the track or -1 if no position was ever set
     */
    double getPositionInSeconds();
private:
    double positionInSeconds = -1.0;
    bool selected = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotCueButton)
};
