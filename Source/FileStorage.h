#pragma once

#include <JuceHeader.h>

class FileStorage
{
public:
    FileStorage(juce::String fileName);

    void write(juce::String content);
    juce::String read();

private:
    static const juce::String FILE_DIRECTORY;

    juce::String fileName;
    juce::File storageFile;
};
