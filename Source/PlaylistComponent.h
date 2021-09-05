/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Aug 2021 11:44:56am
    Author:  Mohammad 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

#include <vector>
#include <string>
#include <map>


//==============================================================================
/*
 A component that displays and performs certain actions on a list of tracks
*/
class PlaylistComponent  : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::FileDragAndDropTarget,
    public juce::TextEditor::Listener
{
public:
    PlaylistComponent(
      DeckGUI* deck,
      juce::AudioFormatManager* formatManager);
    
    ~PlaylistComponent() override;

    /** Called to draw component content */
    void paint (juce::Graphics&) override;
    /** Called when the component size has been changed */
    void resized() override;
    
    // pure virtual member functions from TableListBoxModel class
    /** Returns the number of rows currently in the table */
    int getNumRows () override;
    
    /** Draws the background behind one of the rows in the table  */
    void paintRowBackground (
        juce::Graphics &,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    
    /** Draws one of the cells */
    void paintCell (juce::Graphics &,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    
    /** used to create or update a custom component that goes into a cell */
    Component* refreshComponentForCell (
                            int rowNumber,
                            int columnId,
                            bool isRowSelected,
                            Component *existingComponentToUpdate)
    override;
    
    // pure virtual functions from Button listener class
    /** Called when a button is clicked */
    void buttonClicked (juce::Button* btn) override;
    

    // pure virtual members from the FileDragAndDropTarget class
    /** Callback to check whether this target is interested in the set of files being offered. */
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    /** Callback to indicate that the user has dropped the files onto this component. */
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    
    // functions from text editor listener
    /** Called when the user changes the text in some way */
    void textEditorTextChanged (juce::TextEditor &) override;
    
private:
    // a load button to load multiple tracks
    juce::TextButton loadButton{"Load"};
    
    // a table to display the tracks of a playlist
    juce::TableListBox tableComponent;
    
    // a map to store the title tracks and the url of the track
    std::map<std::string, juce::URL> tracks;
    
    // Deck gui pointer
    DeckGUI* deck;
    
    // search box
    juce::TextEditor searchBox;

    // a map to store the tracks that were not searched for
    std::map<std::string, juce::URL> filteredTracks;
    
    // A manager that keeps a list of available audio formats and
    // decides which one to use to open a given file
    juce::AudioFormatManager* formatManager;
    
    // directory in which the data is going to be stored
    juce::File dataDirectory {
        juce::File::getSpecialLocation(
           juce::File::SpecialLocationType::userDocumentsDirectory
        )
    };
    
    // the file containing the data
    juce::File dataFile {dataDirectory.getFullPathName() + "/data.txt"} ;

    // the data inside the file
    juce::StringArray lines;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
