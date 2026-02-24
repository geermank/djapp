#pragma once

#include <JuceHeader.h>

class FileStorage
{
public:
    // @param fileName is the name of the file used to identify it in storage
    FileStorage(juce::String fileName);

    /** writes the given string in a file stored in disk */
    void write(juce::String content);
    
    /** loads from disk and returns the content of this file as a String */
    juce::String read();

private:
    static const juce::String FILE_DIRECTORY;

    juce::String fileName;
    juce::File storageFile;
};
