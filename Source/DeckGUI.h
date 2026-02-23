/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "DeckConfiguration.h"
#include "Track.h"
#include "HotCueButtonPanel.h"
#include "HotCueButtonInfo.h"
#include <string>

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener,
                   public Timer,
                   public HotCueButtonPanel::Listener
{
public:
    DeckGUI(Track track,
            DeckConfiguration config,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI();
    
    juce::String deckName;

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override; 

    void timerCallback() override; 
    
    /**
     * Loads a new track in this deck, overriding the previous one
     */
    void loadNewTrack(Track newTrack);
    
    /**
     * returns the player instance attached to this deck
     */
    DJAudioPlayer* getPlayer();
    
    /**
     * Creates a new instance of DeckConfiguration with the current config of this deck
     */
    DeckConfiguration getCurrentConfig();
    
    
    void onHotCuePressed(double positionInSeconds) override;
    double getCurrentPositionInSeconds() override;
    
private:
    Track track;

    juce::Label trackName;
    
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    WaveformDisplay waveformDisplay;
    
    HotCueButtonPanel hotCueButtonPanel;

    std::unique_ptr<DJAudioPlayer> player;
    
    std::vector<HotCueButtonInfo> hotCueButtonsInfo;
    
    void updateDeckTitle();
    
    void playTrack();
    void stopTrack();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
