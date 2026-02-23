/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    addAndMakeVisible(deckViewport);
    deckViewport.setViewedComponent(&deckContainer, false);
    deckViewport.setScrollBarsShown(false, true, false, true);
    deckViewport.getHorizontalScrollBar().toFront(false);
    deckViewport.setScrollBarThickness(SCROLLBAR_THICKNESS);;
    
    playlistComponent.setListener(this);
    addAndMakeVisible(playlistComponent);
    
    // setup empty decks label and make it visible
    emptyDecksStateLabel.setText("You haven't created any decks yet!", juce::dontSendNotification);
    emptyDecksStateLabel.setJustificationType(juce::Justification::centred);
    emptyDecksStateLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible(emptyDecksStateLabel);

    formatManager.registerBasicFormats();
        
    restoreAppFromStorage();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentBlockSize = samplesPerBlockExpected;
    currentSampleRate = sampleRate;
    audioPrepared = true;
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    for (int i = 0; i < decks.size(); ++i) {
        DJAudioPlayer* player = decks[i]->getPlayer();
        player->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
 }

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    for (auto& d : decks)
        mixerSource.removeInputSource(d->getPlayer());
    mixerSource.releaseResources();
    
    for (auto& d : decks)
        d->getPlayer()->releaseResources();
    decks.clear();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // we will have two "rows" in the main component: one for the horizontal list of decks and another one for the PlaylistComponent
    int decksHeight = getHeight() * 2/3;
    int playlistHeight = getHeight() / 3;
    int viewportChildrenHeight = decksHeight - SCROLLBAR_THICKNESS;
    
    int firstRowStartY = 0;
    int secondRowStartY = decksHeight;
    
    deckViewport.setBounds(0, firstRowStartY, getWidth(), decksHeight);
    playlistComponent.setBounds(0, secondRowStartY, getWidth(), playlistHeight);
    
    // not a direct child, but shown within the bounds of the viewport
    emptyDecksStateLabel.setBounds(0, firstRowStartY, getWidth(), viewportChildrenHeight);
    
    emptyDecksStateLabel.setVisible(decks.empty());
    deckViewport.setVisible(!decks.empty());

    int deckWidth = 300;
    for (int i = 0; i < decks.size(); ++i)
    {
        decks[i]->setBounds(i * deckWidth, 0, deckWidth, viewportChildrenHeight);
    }

    deckContainer.setSize(decks.size() * deckWidth, viewportChildrenHeight);
}

juce::String MainComponent::loadTrackInANewDeck(Track track) {
    juce::String newDeckName = juce::String("Deck ") + juce::String(decks.size() + 1);
    DeckConfiguration config = DeckConfiguration(newDeckName, track.url, 1.0, 1.0);
    loadTrackInANewDeck(track, config, true);
    return newDeckName;
}

void MainComponent::loadTrackInExistingDeck(Track track, juce::String deckName) {
    
    for (auto& d : decks) {
        if (d->deckName.equalsIgnoreCase(deckName)) {
            d->loadNewTrack(track);
            // save the new state of the decks to storage
            savedDecksToStorage();
            break;
        }
    }
}

void MainComponent::onTracksAddedToPlaylist(std::vector<Track> newTracks) {
    // save all tracks, not only the new ones.
    playlistStorage.save(playlistComponent.getTracks());
}

void MainComponent::restoreAppFromStorage() {
    playlistComponent.addTracks(playlistStorage.get());
    restoreDecksFromStorage();
}

void MainComponent::restoreDecksFromStorage() {
    std::vector<DeckConfiguration> decksConfig = deckStorage.get();
    for (auto& config : decksConfig) {
        auto track = playlistComponent.findTrackByUrl(config.trackUrl);
        loadTrackInANewDeck(track, config, false);
        playlistComponent.registerDeckName(config.name);
    }
}

void MainComponent::savedDecksToStorage() {
    std::vector<DeckConfiguration> decksConfig;
    for (auto& d : decks) {
        decksConfig.push_back(d.get()->getCurrentConfig());
    }
    deckStorage.save(decksConfig);
}

 void MainComponent::loadTrackInANewDeck(Track track,
                                                DeckConfiguration config,
                                                bool saveToStorage) {
    auto deck = std::make_unique<DeckGUI>(track, config, formatManager, thumbCache);
    
    if (audioPrepared) {
        deck->getPlayer()->prepareToPlay(currentBlockSize, currentSampleRate);
    }
    mixerSource.addInputSource(deck->getPlayer(), false);
    
    // add the new deck inside its container
    deckContainer.addAndMakeVisible(*deck);

    // save the deck in the list and force resize to force calculations again
    decks.push_back(std::move(deck));
    
    resized();
    
    // scroll to the latest position, to let the new deck gain focus
    deckViewport.setViewPositionProportionately(1.0, 0.0);
    
    // update decks state in storage if requested
    if (saveToStorage) {
        savedDecksToStorage();
    }
}

void MainComponent::saveProject() {
    playlistStorage.save(playlistComponent.getTracks());
    savedDecksToStorage();
    std::cout << "Project saved automatically" << std::endl;
}
