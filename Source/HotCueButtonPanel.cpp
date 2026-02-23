/*
  ==============================================================================

    HotCueButtonPanel.cpp
    Created: 22 Feb 2026 5:06:58pm
    Author:  German

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HotCueButtonPanel.h"

//==============================================================================
HotCueButtonPanel::HotCueButtonPanel(HotCueButtonPanel::Listener* listener)
{
    this->listener = listener;
    
    for (int i = 0; i < 8; ++i) {
        auto button = std::make_unique<HotCueButton>();
        button.get()->addListener(this);
        addAndMakeVisible(button.get());
        
        hotCueButtons.push_back(std::move(button));
    }
    
    applySelectedHotCueButton.addListener(this);
    setSelectionToHotCueButton.addListener(this);
    clearAllHotCuest.addListener(this);
    
    clearAllHotCuest.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    
    addAndMakeVisible(applySelectedHotCueButton);
    addAndMakeVisible(setSelectionToHotCueButton);
    addAndMakeVisible(clearAllHotCuest);
}

HotCueButtonPanel::~HotCueButtonPanel()
{
    listener = nullptr;
}

void HotCueButtonPanel::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
}

void HotCueButtonPanel::resized()
{
    int hotCueButtonWidth = getWidth() / 4;
    int hotCueButtonHeight = getHeight() / 4;

    for (int i = 0; i < hotCueButtons.size(); ++i)
    {
        // split into 2 rows of 4 colums each
        int col = i % 4;
        int row = i / 4;
        hotCueButtons[i].get()->setBounds(col * hotCueButtonWidth,
                                   row * hotCueButtonHeight,
                                   hotCueButtonWidth,
                                   hotCueButtonHeight);
    }
    
    applySelectedHotCueButton.setBounds(0, hotCueButtonHeight * 2, getWidth() / 2, hotCueButtonHeight);
    setSelectionToHotCueButton.setBounds(getWidth() / 2, hotCueButtonHeight * 2, getWidth() / 2, hotCueButtonHeight);
    clearAllHotCuest.setBounds(0, hotCueButtonHeight * 2 + hotCueButtonHeight, getWidth(), hotCueButtonHeight);
}

void HotCueButtonPanel::buttonClicked (Button* button) {
    if ((button == &applySelectedHotCueButton ||
        button == &setSelectionToHotCueButton ||
        button == &clearAllHotCuest) &&
        selectedButton == nullptr) {
        return;
    }
    
    if (button == &applySelectedHotCueButton) {
        double position = selectedButton->getPositionInSeconds();
        if (position >= 0) {
            listener->onHotCuePressed(position);
        }
        return;
    } else if (button == &setSelectionToHotCueButton) {
        selectedButton->setPositionInSeconds(listener->getCurrentPositionInSeconds());
        return;
    } else if (button == &clearAllHotCuest) {
        for (int i = 0; i < hotCueButtons.size(); ++i) {
            hotCueButtons[i].get()->clearPosition();
        }
        return;
    }
    
    // if any of the cases above succeed, then the button click must have happened in a hot cue button
    HotCueButton* hotCueButton = dynamic_cast<HotCueButton*>(button);
    if (hotCueButton == nullptr)
    {
        return;
    }
    hotCueButton->setSelected(!hotCueButton->isSelected());
    
    bool selectedOn = hotCueButton->isSelected();
    if (!selectedOn) {
        // if selectedOn is false, the it means that the clicked hot cue
        // was previously selected and now it was turned off. Remove the panel ref to it
        selectedButton = nullptr;
        return;
    }
    
    // if the clicked button was turned on, turn off the previous selection if available
    if (selectedButton != nullptr) {
        selectedButton->setSelected(false);
    }
    
    // keep the reference to the currently active hot cue button
    selectedButton = hotCueButton;
}

std::vector<HotCueButtonInfo> HotCueButtonPanel::getHotCuesInfo() {
    std::vector<HotCueButtonInfo> hotCues;
    for(int i = 0; i < hotCueButtons.size(); ++i) {
        auto button = hotCueButtons[i].get();
        double position = button->getPositionInSeconds();
        hotCues.push_back(HotCueButtonInfo(i, position));
    }
    return hotCues;
}

void HotCueButtonPanel::restoreHotCues(std::vector<HotCueButtonInfo> hotCues) {
    for (const auto& hotCue : hotCues)
    {
        if (hotCue.order >= 0 &&
            hotCue.order < hotCueButtons.size() &&
            hotCue.positionInSeconds >= 0.0)
        {
            hotCueButtons[hotCue.order]->setPositionInSeconds(hotCue.positionInSeconds);
        }
    }
}
