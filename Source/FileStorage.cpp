#include "FileStorage.h"
#include <fstream>
#include <iostream>

const juce::String FileStorage::FILE_DIRECTORY = "OtoDecks/files/";

FileStorage::FileStorage(juce::String fileName) : fileName(fileName)
{
    auto baseDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                              .getChildFile(FILE_DIRECTORY);
    baseDir.createDirectory();
    storageFile = baseDir.getChildFile(fileName);
}

void FileStorage::write(juce::String content)
{
    std::ofstream out(storageFile.getFullPathName().toStdString(), std::ios::trunc);
    if (out.is_open())
    {
        out << content.toStdString();
    }
    else
    {
        std::cout << "FileStorage::write could not open file " << fileName << std::endl;
    }
}

juce::String FileStorage::read()
{
    std::ifstream f(storageFile.getFullPathName().toStdString());
    if (!f.is_open())
        return "";

    std::string content;
    std::string line;
    while (std::getline(f, line))
    {
        content += line + "\n";
    }
    return juce::String(content);
}
