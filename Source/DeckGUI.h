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
    //======================= Constructor / Destructor ==========================
    DeckGUI(Track track,
            DeckConfiguration config,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI();
    
    //=============================== Component =================================
    void paint (Graphics&) override;
    void resized() override;
    
    //=========================== Button::Listener ==============================
   void buttonClicked (Button *) override;

    //=========================== Slider::Listener ==============================
    void sliderValueChanged (Slider *slider) override;

    //=========================== Timer::Listener ==============================
    void timerCallback() override;
    
    //=========================== Public properties =============================
    juce::String deckName;

    //============================ Public methods ===============================
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
    
    /** hot cue panel listener methods */
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
    
    Slider lowEqSlider;
    Slider midEqSlider;
    Slider highEqSlider;

    WaveformDisplay waveformDisplay;
    
    HotCueButtonPanel hotCueButtonPanel;

    std::unique_ptr<DJAudioPlayer> player;
    
    std::vector<HotCueButtonInfo> hotCueButtonsInfo;
    
    void updateDeckTitle();
    
    void playTrack();
    void stopTrack();
    
    void setUpEqSlider(Slider* slider, double value);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
