#include "TrackFactory.h"
#include "TimeFormat.h"

Track TrackFactory::createTrackFromFile(juce::String fileUrl,
                                        juce::AudioFormatManager& formatManager) {
    juce::File f(fileUrl);
    return createTrackFromFile(f, formatManager);
}

Track TrackFactory::createTrackFromFile(juce::File file,
                                        juce::AudioFormatManager& formatManager) {
    juce::String name = file.getFileName();
    juce::String url = file.getFullPathName();
    
    double durationInSeconds = calculateDurationInSeconds(file, formatManager);
    juce::String duration = TimeFormat::formatTrackDuration(durationInSeconds);
    
    return Track{name, url, duration};
}


double TrackFactory::calculateDurationInSeconds(juce::File& f,
                                              juce::AudioFormatManager& formatManager) {
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(f));
    if (!reader)
        return -1;

    return reader->lengthInSamples / reader->sampleRate;
}
