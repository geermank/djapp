/*
  ==============================================================================

    PlaylistStorage.h
    Created: 20 Feb 2026 7:50:08pm
    Author:  German

  ==============================================================================
*/

#pragma once

#import "Track.h"
#import "FileStorage.h"

class PlaylistStorage {
    
public:
    /**
     * returns a list of stored tracks or empty if there are no tracks saved or an error occurred
     */
    std::vector<Track> get();
    
    /**
     * saves the given list of tracks to the storage. This will override the previous saved list
     * returns true if data could be saved, false otherwise
     */
    bool save(std::vector<Track> tracks);
private:
    FileStorage storage{"playlist"};
};
