/*
  ==============================================================================

    DeckStorage.cpp
    Created: 20 Feb 2026 7:49:46pm
    Author:  German

  ==============================================================================
*/

#include "DeckStorage.h"

std::vector<DeckConfiguration> DeckStorage::get() {
    std::vector<DeckConfiguration> result;
    try {
        auto content = storage.read();
        if (!content.isEmpty()) {
            result = DeckConfigurationParser::deserialize(content);
        }
    } catch(std::exception error) {
        std::cout << "Error while reading deck storage" << std::endl;
    }
    return result;
}

bool DeckStorage::save(std::vector<DeckConfiguration> configs) {
    bool success;
    try {
        juce::String contentToSave = DeckConfigurationParser::serialize(configs);
        storage.write(contentToSave);
        success = true;
    } catch(...) {
        std::cout << "Error saving to deck storage" << std::endl;
        success = false;
    }
    return success;
}
