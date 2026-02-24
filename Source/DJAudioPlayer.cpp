#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
    : formatManager(_formatManager) {}

DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 1;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    setLowGain(0.0f);
    setMidGain(0.0f);
    setHighGain(0.0f);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    auto* buffer = bufferToFill.buffer;
    auto numSamples = bufferToFill.numSamples;
    auto startSample = bufferToFill.startSample;

    if (buffer->getNumChannels() > 0)
    {
        juce::dsp::AudioBlock<float> leftBlock(buffer->getArrayOfWritePointers(),
                                               1, startSample, numSamples);
        juce::dsp::ProcessContextReplacing<float> leftCtx(leftBlock);
        leftChain.process(leftCtx);
    }

    if (buffer->getNumChannels() > 1)
    {
        auto* rightPtr = buffer->getArrayOfWritePointers() + 1;
        juce::dsp::AudioBlock<float> rightBlock(rightPtr,
                                                1, startSample, numSamples);
        juce::dsp::ProcessContextReplacing<float> rightCtx(rightBlock);
        rightChain.process(rightCtx);
    }
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio <= 0 || ratio > 2.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

bool DJAudioPlayer::isPlaying()
{
    return transportSource.isPlaying();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getPositionInSeconds()
{
    return transportSource.getCurrentPosition();
}

void DJAudioPlayer::setLowGain(float gainDb)
{
    auto coefs = FilterCoefs::makeLowShelf(currentSampleRate, 300.0f, 0.7f,
                                           juce::Decibels::decibelsToGain(gainDb));
    *leftChain.get<0>().coefficients  = *coefs;
    *rightChain.get<0>().coefficients = *coefs;
}

void DJAudioPlayer::setMidGain(float gainDb)
{
    auto coefs = FilterCoefs::makePeakFilter(currentSampleRate, 1000.0f, 0.7f,
                                             juce::Decibels::decibelsToGain(gainDb));
    *leftChain.get<1>().coefficients  = *coefs;
    *rightChain.get<1>().coefficients = *coefs;
}

void DJAudioPlayer::setHighGain(float gainDb)
{
    auto coefs = FilterCoefs::makeHighShelf(currentSampleRate, 5000.0f, 0.7f,
                                            juce::Decibels::decibelsToGain(gainDb));
    *leftChain.get<2>().coefficients  = *coefs;
    *rightChain.get<2>().coefficients = *coefs;
}
