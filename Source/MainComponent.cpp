/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "TimeFormat.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

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
    
    setUpDeckViewportComponent();
    setUpPlaylistComponent();
    setUpEmptyStateComponent();
    setUpSaveProjectComponent();
    
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
    // we will have three "rows" in the main component: one for the horizontal list of decks, another one for the PlaylistComponent, and the last one for the save project feature
    
    // first calculate components height
    int decksHeight = getHeight() * 15/20;
    int deckViewportChildrenHeight = decksHeight - SCROLLBAR_THICKNESS;
    
    int playlistHeight = getHeight() * 4/20;
    int autoSaveActionsHeight = getHeight() * 1/20;

    // components y-axis coordinate
    int decksRowStartY = 0;
    int playlistRowStartY = decksHeight;
    int saveProjectFeatureStartY = decksHeight + playlistHeight;
    
    deckViewport.setBounds(0, decksRowStartY, getWidth(), decksHeight);
    // empty state label is not a direct child of the viewport, but it is shown within its bounds
    emptyDecksStateLabel.setBounds(0, decksRowStartY, getWidth(), deckViewportChildrenHeight);
    playlistComponent.setBounds(0, playlistRowStartY, getWidth(), playlistHeight);
    
    int deckWidth = 300;
    for (int i = 0; i < decks.size(); ++i)
    {
        decks[i]->setBounds(i * deckWidth, 0, deckWidth, deckViewportChildrenHeight);
    }

    deckContainer.setSize(decks.size() * deckWidth, deckViewportChildrenHeight);
    
    nextAutoSaveLabel.setBounds(getWidth() - 455, saveProjectFeatureStartY, 300, autoSaveActionsHeight);
    saveButton.setBounds(getWidth() - 150, saveProjectFeatureStartY, 150, autoSaveActionsHeight);
}

juce::String MainComponent::loadTrackInANewDeck(Track track) {
    juce::String newDeckName = juce::String("Deck ") + juce::String(decks.size() + 1);
    DeckConfiguration config = DeckConfiguration{newDeckName,track.url};
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

    // update empty state and decks visibility accordingly
    deckViewport.setVisible(!decks.empty());
    emptyDecksStateLabel.setVisible(decks.empty());
    
    // update decks state in storage if requested
    if (saveToStorage) {
        savedDecksToStorage();
    }
     
    // call resize needed to display items correctly
    resized();
     
    // scroll to the latest position, to let the new deck gain focus
    deckViewport.setViewPositionProportionately(1.0, 0.0);
}

void MainComponent::saveProject() {
    playlistStorage.save(playlistComponent.getTracks());
    savedDecksToStorage();
    std::cout << "Project saved" << std::endl;
}

void MainComponent::onAutoSaveExecuted(juce::String lastSaveTime, juce::String nextSaveTime) {
    nextAutoSaveLabel.setText("Last saved at " + lastSaveTime +  + ". Next auto-save at " + nextSaveTime, juce::dontSendNotification);
}

void MainComponent::buttonClicked(Button* button) {
    saveProject();
    
    juce::String nowTimeFormatted = TimeFormat::formatTimeAsHHMM(juce::Time::getCurrentTime());
    onAutoSaveExecuted(nowTimeFormatted, getNextAutoSaveTime());
}

void MainComponent::setUpDeckViewportComponent() {
    addAndMakeVisible(deckViewport);
    deckViewport.setViewedComponent(&deckContainer, false);
    deckViewport.setScrollBarsShown(false, true, false, true);
    deckViewport.getHorizontalScrollBar().toFront(false);
    deckViewport.setScrollBarThickness(SCROLLBAR_THICKNESS);
}

void MainComponent::setUpPlaylistComponent() {
    playlistComponent.setListener(this);
    addAndMakeVisible(playlistComponent);
}

void MainComponent::setUpEmptyStateComponent() {
    emptyDecksStateLabel.setText("You haven't created any decks yet!", juce::dontSendNotification);
    emptyDecksStateLabel.setJustificationType(juce::Justification::centred);
    emptyDecksStateLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible(emptyDecksStateLabel);
}

void MainComponent::setUpSaveProjectComponent() {
    addAndMakeVisible(nextAutoSaveLabel);
    nextAutoSaveLabel.setText("Project not saved", juce::dontSendNotification);
    nextAutoSaveLabel.setJustificationType(juce::Justification::centredRight);
    
    addAndMakeVisible(saveButton);
    saveButton.addListener(this);
}
