/*
  ==============================================================================

    PlaylistStorage.cpp
    Created: 20 Feb 2026 7:50:08pm
    Author:  German

  ==============================================================================
*/

#include "PlaylistStorage.h"
#include "TrackParser.h"

std::vector<Track> PlaylistStorage::get() {
    std::vector<Track> result;
    try {
        auto content = storage.read();
        if (!content.isEmpty()) {
            result = TrackParser::deserialize(content);
        }
    } catch(...) {
        std::cout << "Error reading tracks from storage" << std::endl;
    }
    return result;
}

bool PlaylistStorage::save(std::vector<Track> tracks) {
    bool success;
    try {
        juce::String content = TrackParser::serialize(tracks);
        storage.write(content);
        success = true;
    } catch(...) {
        std::cout << "Error writing tracks to storage" << std::endl;
        success = false;
    }
    return success;
}
