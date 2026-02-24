#pragma once

#include <JuceHeader.h>
#include "HotCueButton.h"
#include "HotCueButtonInfo.h"

class HotCueButtonPanel  : public juce::Component,
                           public Button::Listener
{
public:
    /**
     * Listener class to observe important events triggered in the panel
     */
    class Listener {
    public:
        virtual ~Listener() = default;
        
        virtual void onHotCuePressed(double positionInSeconds) = 0;
        virtual double getCurrentPositionInSeconds() = 0;
    };
    //========================= Constructor / Destructor ========================
    HotCueButtonPanel(HotCueButtonPanel::Listener* listener);
    ~HotCueButtonPanel() override;
    
    //=============================== Button::Listener ==========================
    void buttonClicked (Button* button) override;

    //=============================== Component =================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //=============================== Public methods ============================
    /** returns the state of all the hot cue buttons in the panel */
    std::vector<HotCueButtonInfo> getHotCuesInfo();
    
    /** recreates the state of the hot cue buttons from the given data */
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
