/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "PlaylistStorage.h"
#include "AutoSave.h"
#include "DeckStorage.h"
#include <string>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public PlaylistComponent::Listener,
                      public AutoSave,
                      public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    //=========================== Playlist::Listener ===============================
    void loadTrackInExistingDeck(Track track, juce::String deckName) override;
    juce::String loadTrackInANewDeck(Track track) override;
    void onTracksAddedToPlaylist(std::vector<Track> tracks) override;
    
    //============================ AutoSave methods =================================
    void saveProject() override;
    void onAutoSaveExecuted(juce::String lastSaveTime, juce::String nextSaveTime) override;
    
    //=========================== Butt::Listener ===============================
    void buttonClicked(Button* button) override;
    
private:
    // the thickness of the scrollbar shown in the deck viewport
    // which is also used to calculate the height of the viewport
    static const int SCROLLBAR_THICKNESS = 12;
    
    // audio classes
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};
    MixerAudioSource mixerSource;
    
    /**
     * A list of decks that are being displayed in the console. These are created dynamically by the user
     */
    std::vector<std::unique_ptr<DeckGUI>> decks;
    
    /**
     * Components used as contaniers of the deck. This allows us to enable horizontal scrolling
     */
    juce::Viewport deckViewport;
    juce::Component deckContainer;
    
    /**
     * The component that displays a list of tracks the DJ has loaded
     */
    PlaylistComponent playlistComponent{formatManager};
    
    /**
     * A label displayed when the user has yet not added decks
     */
    juce::Label emptyDecksStateLabel;
    
    /** Label that shows to the user when the next auto save is going to happen */
    juce::Label nextAutoSaveLabel;
    /** Button to allow the user to save the project manually */
    juce::TextButton saveButton{"Save project"};
    
    /**
     * Storage classes to save the state of the playlist and the decks
     */
    PlaylistStorage playlistStorage;
    DeckStorage deckStorage;
    
    /**
     * properties initialized when prepareToPlay method is called.
     * Used to prepare to play new players as we dynamically create them
     */
    int currentBlockSize = 0;
    double currentSampleRate = 0.0;
    bool audioPrepared = false;
    
    /**
     * methods to setup components before drawing
     */
    void setUpDeckViewportComponent();
    void setUpPlaylistComponent();
    void setUpEmptyStateComponent();
    void setUpSaveProjectComponent();
    
    /**
     * Methods to save and load data from storage
     */
    void restoreAppFromStorage();
    void restoreDecksFromStorage();
    void savedDecksToStorage();
    
    /**
     * Method to load a new track in a new deck, using the provided configuration
     */
    void loadTrackInANewDeck(Track track, DeckConfiguration config, bool saveToStorage);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
