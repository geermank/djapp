#pragma once

#include <JuceHeader.h>
#include "Track.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public FileDragAndDropTarget
{
public:
    //======================= Constructor / Destructor ==========================
    PlaylistComponent(AudioFormatManager & formatManager);
    ~PlaylistComponent() override;
    
    //=============================== Listener ==================================
    /**
     * Listener class to receive events from PlaylistComponent
     */
    class Listener
    {
    public:
        virtual ~Listener() = default;
        
        /**
         * Notifies when a list of tracks was added to this playlist
         */
        virtual void onTracksAddedToPlaylist(std::vector<Track> newTracks) = 0;

        /**
         * Request to the listener to load the given track in a new deck. Return the deck name
         */
        virtual juce::String loadTrackInANewDeck(Track track) = 0;
        
        /**
         * Request to the listener to load the given track in an existing deck
         */
        virtual void loadTrackInExistingDeck(Track track, juce::String deckName) = 0;
    };

    //========================== Component methods =================================
    void paint (juce::Graphics&) override;
    void resized() override;
 
    //========================== TableList methods =================================
    int getNumRows () override;
    
    void paintRowBackground (Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    
    void paintCell (Graphics &,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component *existingComponentToUpdate) override;
    
    //======================== Button listener method ===============================
    void buttonClicked(Button* button) override;
    
    
    //======================= File drag and drop methods ============================
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    
    
    //======================= PlaylistComponent methods ============================
    /**
     * Register a new listener instance in the playlist, which will be used to notify about important events or
     * require actions like loading a new deck with a specific trakc.
     */
    void setListener(Listener* newListener) { listener = newListener; }
    
    /**
     * Shows the file chooser to the user and tries to add a new track
     */
    void addTrackWithFileChooser();
    
    /**
     * returns a list of tracks currently available in this playlist
     */
    std::vector<Track> getTracks();
    
    /**
     * returns a specific track by url. Can throw if the track is not in the playlist
     */
    Track findTrackByUrl(juce::String url);
    
    /**
     * adds a list of tracks to this playlist
     */
    void addTracks(std::vector<Track> newTracks);
    
    /**
     * lets this playlist component know about a particular deck
     */
    void registerDeckName(juce:: String name);
private:
    /**
     * all the tracks that have been loading to the playlist
     */
    std::vector<Track> tracks;
    
    /**
     * a list of the decks available
     */
    std::vector<juce::String> deckNames;

    /**
     * Label to indicate the user that he hasn't load any tracks yet
     */
    juce::Label emptyStateLabel;
    /**
     * Button to prompt the user to load more tracks using a button
     */
    juce::TextButton addFilesButton;
    
    /**
     * the table component used to display the list of tracks
     */
    TableListBox tableComponent;
    
    /**
     * The listener instance to send events to
     */
    Listener* listener = nullptr;
    
    /**
     * Uses format manager to determine if dropped files are valid/known audio types
     */
    AudioFormatManager& formatManager;
    
    /**
     * Used to load files using into the playlist, to avoid depending only on the drag&drop
     */
    FileChooser fileChooser = FileChooser{"Select the audio files you want to work with"};
    
    /**
     * Methods to set up the internal components of the playlist during construction
     */
    void setUpTableComponent();
    void setUpEmptyStateComponent();
    void setUpAddFilesComponent();
    
    /**
     * Runs UI tasks required to show the recently added tracks
     */
    void updateUiAfterNewTrackWasLoaded();
    
    /**
     * adds tracks to the list and notifies observers if asked to
     */
    void addTracks(std::vector<Track> newTracks, bool notify);
    
    /**
     * Shows a popup menu at the given row
     * @param rowComponentTarget the component on top of which the popup menu has to be shown
     */
    void showPopupMenuForRow(Component* rowComponentTarget);
    
    /**
     * returns true if the file was added to the list of tracks, false otherwise
     */
    bool isTrackAlreadyAdded(juce::String filePath);
    
    /** returns true if it is a valid audio file format, flase otherwise */
    bool isValidFormat(File f);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
