/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(Track track,
                 DeckConfiguration config,
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : deckName(config.name),
                track(track),
                waveformDisplay(formatManagerToUse, cacheToUse),
                hotCueButtonPanel(this)
{
    player = std::make_unique<DJAudioPlayer>(formatManagerToUse);
    
    addAndMakeVisible(trackName);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);
    
    addAndMakeVisible(hotCueButtonPanel);

    playButton.addListener(this);
    stopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(config.volume);
    
    speedSlider.setRange(0.5, 2.0);
    speedSlider.setValue(config.speed);
    
    posSlider.setRange(0.0, 1.0);
    
    startTimer(500);
    
    loadNewTrack(track);
    updateDeckTitle();
    hotCueButtonPanel.restoreHotCues(config.hotCues);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 10;
    trackName.setBounds(0, 0, getWidth(), rowH);
    playButton.setBounds(0, rowH, getWidth() / 2, rowH);
    stopButton.setBounds(getWidth() / 2, rowH, getWidth() / 2, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    hotCueButtonPanel.setBounds(0, rowH * 7, getWidth(), rowH * 3);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        playTrack();
    }
     if (button == &stopButton)
    {
        stopTrack();
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

DJAudioPlayer* DeckGUI::getPlayer() {
    return player.get();
}

void DeckGUI::loadNewTrack(Track newTrack) {
    stopTrack();
    
    track = newTrack;
    
    juce::File f(newTrack.url);
    juce::URL url(f);
    
    player->loadURL(url);
    waveformDisplay.loadURL(url);
    
    updateDeckTitle();
}

void DeckGUI::updateDeckTitle() {
    trackName.setText(track.name + " (" + juce::String(deckName) + ")", juce::dontSendNotification);
}

void DeckGUI::playTrack() {
    if (player->isPlaying()) {
        return;
    }
    std::cout << "Playing track" << std::endl;
    player->start();
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
}
void DeckGUI::stopTrack() {
    if (!player->isPlaying()) {
        return;
    }
    std::cout << "Stopping track" << std::endl;
    player->stop();
    playButton.removeColour(juce::TextButton::buttonColourId);
}

DeckConfiguration DeckGUI::getCurrentConfig() {
    auto hotCues = hotCueButtonPanel.getHotCuesInfo();
    return DeckConfiguration{
        deckName,
        track.url,
        speedSlider.getValue(),
        volSlider.getValue(),
        hotCues
    };
}

void DeckGUI::onHotCuePressed(double positionInSeconds) {
    player->setPosition(positionInSeconds);
}

double DeckGUI::getCurrentPositionInSeconds() {
    return player->getPositionInSeconds();
}
