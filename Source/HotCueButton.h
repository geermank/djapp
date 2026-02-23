/*
  ==============================================================================

    HotCueButton.h
    Created: 22 Feb 2026 4:50:28pm
    Author:  German

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class HotCueButton  : public juce::TextButton
{
public:
    HotCueButton();
    ~HotCueButton() override;

    void setSelected(bool val);
    bool isSelected();
    
    void clearPosition();
    void setPositionInSeconds(double seconds);
    double getPositionInSeconds();
private:
    double positionInSeconds = -1.0;
    bool selected = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotCueButton)
};
