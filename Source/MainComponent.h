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
                      public AutoSave
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
    //==============================================================================
    void loadTrackInExistingDeck(Track track, juce::String deckName) override;
    juce::String loadTrackInANewDeck(Track track) override;
    void onTracksAddedToPlaylist(std::vector<Track> tracks) override;
    
    void saveProject() override;

private:
    static const int SCROLLBAR_THICKNESS = 12;
    
    //==============================================================================
    // Your private member variables go here...
     
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
    
    void restoreAppFromStorage();
    void restoreDecksFromStorage();
    void savedDecksToStorage();
    
    void loadTrackInANewDeck(Track track, DeckConfiguration config, bool saveToStorage);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
