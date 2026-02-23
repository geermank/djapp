#pragma once

#include <JuceHeader.h>
#include "HotCueButton.h"
#include "HotCueButtonInfo.h"

class HotCueButtonPanel  : public juce::Component,
                           public Button::Listener
{
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        
        virtual void onHotCuePressed(double positionInSeconds) = 0;
        virtual double getCurrentPositionInSeconds() = 0;
    };
    
    HotCueButtonPanel(HotCueButtonPanel::Listener* listener);
    ~HotCueButtonPanel() override;
    
    void buttonClicked (Button* button) override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::vector<HotCueButtonInfo> getHotCuesInfo();
    
    void restoreHotCues(std::vector<HotCueButtonInfo> hotCues);
    
private:
    std::vector<std::unique_ptr<HotCueButton>> hotCueButtons;
    HotCueButton* selectedButton = nullptr;
    
    TextButton applySelectedHotCueButton{"Apply"};
    TextButton setSelectionToHotCueButton{"Set time"};
    TextButton clearAllHotCuest{"Clear all"};
    
    Listener* listener;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotCueButtonPanel)
};
