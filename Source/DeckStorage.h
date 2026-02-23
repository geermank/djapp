#pragma once

#import "DeckConfiguration.h"
#import "DeckConfigurationParser.h"
#import "FileStorage.h"

class DeckStorage {
    
public:
    /**
     * returns a list of DeckConfiguration or empty if there are no deck configurations saved yet or an error occurred
     */
    std::vector<DeckConfiguration> get();
    
    /**
     * saves the given list of deck configurations, overwriting the previous saved data. Returns true if it is successful, false otherwise.
     */
    bool save(std::vector<DeckConfiguration> configs);
private:
    FileStorage storage{"decks"};
};
