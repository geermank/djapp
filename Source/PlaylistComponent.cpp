/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 15 Feb 2026 5:37:49pm
    Author:  German

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "TrackFactory.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager & formatManager) : formatManager(formatManager)
{
    setUpTableComponent();
    setUpEmptyStateComponent();
    setUpAddFilesComponent();
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    
    int labelWidth = 300;
    int labelHeight = 50;
    int labelX = getWidth() / 2 - labelWidth / 2;
    int labelY = getHeight() / 2 - labelHeight;
    emptyStateLabel.setBounds(labelX, labelY, labelWidth, labelHeight);
    
    int paddingBetweenButtonAndLabel = 10;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = labelX + (labelWidth - buttonWidth) / 2;
    int buttonY = labelY + labelHeight + paddingBetweenButtonAndLabel;
    addFilesButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
}

int PlaylistComponent::getNumRows ()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground (Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    // no-op
}

void PlaylistComponent::paintCell (Graphics & g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    auto track = tracks[rowNumber];
    if (columnId == 1) {
        g.setColour(Colours::white);
        g.drawText (track.name,
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
    } else if (columnId == 2) {
        g.setColour(Colours::white);
        g.drawText (track.duration,
                    2, 0,
                    width - 4, height,
                    Justification::centredLeft,
                    true);
    }
}

Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                                       int columnId,
                                                       bool isRowSelected,
                                                       Component *existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton("Actions");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        existingComponentToUpdate->setComponentID(juce::String(rowNumber));
    }
    return existingComponentToUpdate;
}

bool PlaylistComponent::isInterestedInFileDrag (const StringArray &files)
{
    // Accept multiple files, but only when all are valid types
    for (auto file : files)
    {
        juce::File f(file);
        if (!isValidFormat(f)) {
            // if there is no reader for this file in the audio format manager
            // then we can't process it, so ignore it
            return false;
        }
    }
    return true;
}

void PlaylistComponent::filesDropped(const StringArray &files, int x, int y)
{
    std::vector<Track> newTracks;
    for (auto file : files)
    {
        if (!isTrackAlreadyAdded(file)) {
            newTracks.push_back(TrackFactory::createTrackFromFile(file, formatManager));
        }
    }
    addTracks(newTracks, true);
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &addFilesButton) {
        addTrackWithFileChooser();
    } else {
        showPopupMenuForRow(button);
    }
}

void PlaylistComponent::showPopupMenuForRow(Component* rowComponentTarget){
    // documentation: https://docs.juce.com/develop/classjuce_1_1PopupMenu.html
    int row = rowComponentTarget->getComponentID().getIntValue();
    
    juce::PopupMenu menu;
    menu.addItem(1, "Load into New Deck");
    
    if (!deckNames.empty()) {
        menu.addSeparator();
        for (int i = 0; i < deckNames.size(); ++i) {
            auto deckName = deckNames[i];
            
            // add 1 to avoid zero index (taken by no-selection case)
            // multiply by 10 to avoid 1-index, taken by new deck case
            int deckIdInPopupMenu = (i + 1) * 10;
            
            menu.addItem(deckIdInPopupMenu, deckName);
        }
    }
    
    menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(rowComponentTarget), [this, row](int result) {
        if (result == 0) return;
        if (listener == nullptr) return;
        
        if (result == 1) {
            juce::String newDeckName = listener->loadTrackInANewDeck(tracks[row]);
            deckNames.push_back(newDeckName);
        } else {
            int unpackedDeckId = (result / 10) - 1;
            auto deckName = deckNames[unpackedDeckId];
            listener->loadTrackInExistingDeck(tracks[row], deckName);
        }
    });
}

void PlaylistComponent::addTrackWithFileChooser(){
    // documentation: https://docs.juce.com/develop/classjuce_1_1FileChooser.html
    auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    fileChooser.launchAsync (flags, [this] (const FileChooser& chooser)
    {
        File file (chooser.getResult());
        if (!file.existsAsFile()) {
            // not a file
            return;
        }
        if (!isValidFormat(file)) {
            // can't read this kind of files, abort
            return;
        }
        if (isTrackAlreadyAdded(file.getFullPathName())) {
            // already added, don't duplicate tracks
            return;
        }
        std::vector<Track> trackResult;
        trackResult.push_back(TrackFactory::createTrackFromFile(file, formatManager));
        addTracks(trackResult, true);
        
        updateUiAfterNewTrackWasLoaded();
    });
}

void PlaylistComponent::updateUiAfterNewTrackWasLoaded() {
    tableComponent.updateContent();
    emptyStateLabel.setVisible(tracks.empty());
    addFilesButton.setVisible(tracks.empty());
}

bool PlaylistComponent::isTrackAlreadyAdded(juce::String filePath)
{
    for (const auto& track : tracks)
    {
        if (track.url == filePath)
            return true;
    }
    return false;
}

std::vector<Track> PlaylistComponent::getTracks() {
    return tracks;
}

void PlaylistComponent::addTracks(std::vector<Track> newTracks) {
    addTracks(newTracks, false);
}

void PlaylistComponent::addTracks(std::vector<Track> newTracks, bool notify) {
    for (auto &t : newTracks) {
        tracks.push_back(t);
    }
    updateUiAfterNewTrackWasLoaded();
    if (notify && listener) {
        listener->onTracksAddedToPlaylist(newTracks);
    }
}

Track PlaylistComponent::findTrackByUrl(juce::String url) {
    for (auto& t : tracks) {
        if (t.url == url) {
            return t;
        }
    }
    throw std::exception();
}

void PlaylistComponent::registerDeckName(juce:: String name) {
    deckNames.push_back(name);
}

void PlaylistComponent::setUpTableComponent() {
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Tile", 1, 400);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.setModel(this);
}

void PlaylistComponent::setUpEmptyStateComponent() {
    emptyStateLabel.setText("Drop your audio files here or click the button below", juce::dontSendNotification);
    emptyStateLabel.setJustificationType(juce::Justification::centred);
    emptyStateLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    emptyStateLabel.setVisible(tracks.empty());
    addAndMakeVisible(emptyStateLabel);
}

void PlaylistComponent::setUpAddFilesComponent() {
    addFilesButton.setButtonText("Add files");
    addFilesButton.addListener(this);
    addFilesButton.setVisible(tracks.empty());
    addAndMakeVisible(addFilesButton);
}

bool PlaylistComponent::isValidFormat(File f) {
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(f));
    return reader != nullptr;
}
