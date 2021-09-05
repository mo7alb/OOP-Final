/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Aug 2021 11:44:56am
    Author:  Mohammad 

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(
    DeckGUI* _deck,
    juce::AudioFormatManager* _formatManager
) : deck(_deck),
    formatManager(_formatManager)
{
    // check if the data file exits or not
    if (!dataFile.existsAsFile()) { // data file does not exists
        // create a new file
        auto result = dataFile.create();
        // check if file was succesfully created
        if (result.failed()) {
            // print error - if any
            std::cout << result.getErrorMessage() << std::endl;
        }
    }
    
    // read data from the data file
    dataFile.readLines(lines);
    // check if there is any data
    if (lines.size() != 0) {
        // iterate over the lines of the data file
        for (auto& line : lines) {
            // check if line is empty
            if (line != "") { // line is not empty
                // create a url from the string
                juce::URL trackURL {line};
                // create a file from the url
                juce::File trackFile {trackURL.getLocalFile()};
                // create a pair for the name and url of the track
                std::pair<std::string, juce::URL> savedTrack;
                savedTrack.first = trackFile.getFileName().toStdString();
                savedTrack.second = trackURL;
                
                // insert the saved track to the tracks list
                tracks.insert(savedTrack);
            } // end if
        } // end for
    } // end if
    
    // add and the load button visible
    addAndMakeVisible(loadButton);
    // add and make the search box visible
    addAndMakeVisible(searchBox);
    // add and make the table list component visible
    addAndMakeVisible(tableComponent);
    
    // add listener to the search box
    searchBox.addListener(this);
    // add event listener to the load button
    loadButton.addListener(this);
    // set the model of the table
    tableComponent.setModel(this);
    
    // add a column to the table for the track titles
    tableComponent.getHeader().addColumn("Track title", 1, 550);
    // add a column to the table for the play button
    tableComponent.getHeader().addColumn("", 2, 175);
    
    // disable multiline feature
    searchBox.setMultiLine(false);
    
    // set the background colour of the table to grey
    getLookAndFeel().setColour(juce::TableListBox::backgroundColourId, juce::Colours::grey);
    // set the background colour of the search box to light grey
    getLookAndFeel().setColour(juce::TextEditor::backgroundColourId, juce::Colours::grey);
    // set the background colour of the search box to light grey
    getLookAndFeel().setColour(juce::TextEditor::outlineColourId, juce::Colours::black);
    // set the background colour of the search box to light grey
    getLookAndFeel().setColour(juce::TextEditor::focusedOutlineColourId, juce::Colours::black);
    // set the background colour of the search box to light grey
    getLookAndFeel().setColour(juce::TextEditor::textColourId, juce::Colours::black);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (juce::Colours::grey);

    g.setColour (juce::Colours::grey);
    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);
}

void PlaylistComponent::resized()
{
    // set the size of the load button -- to be changed in the future
    loadButton.setBounds(10, 5, getWidth() / 4, getHeight() / 9 - 5);
    
    // set the size of the search box
    searchBox.setBounds(getWidth() - getWidth()/3 - 10, 5, getWidth() / 3, getHeight()/12);
    
    // set the size of the table component (it takes the whole area)
    tableComponent.setBounds(0, getHeight()/8, getWidth(), getHeight());
}

// function that returns the number of rows currently in the table
int PlaylistComponent::getNumRows () {
    return static_cast<int>(tracks.size());
}

// Draws the background behind one of the rows in the table
void PlaylistComponent::paintRowBackground (
    juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected
) {
    // Display orange color for the backgrounds of the rows selected
    if (rowIsSelected) {
        g.fillAll(juce::Colours::grey);
    } // end of if
    // If not selected, Set the color to dark grey
    else {
        g.fillAll(juce::Colours::white);
    } // end of else
} // end of function

// Draws one of the cells
void PlaylistComponent::paintCell (
   juce::Graphics& g,
   int rowNumber,
   int columnId,
   int width,
   int height,
   bool rowIsSelected
) {
    // integer to keep track of the row number
    unsigned int counter = 0;
    // iterate over all the tracks
    for (auto& item: tracks) {
        // if rowNumber is equal to the row counter display the
        // track name at the respective row
        if (counter == rowNumber) {
            // draw the track title
            g.drawText(item.first,
                       2,
                       0,
                       width - 4,
                       height,
                       juce::Justification::centredLeft,
                       true);
            // break out of the loop
            break;
        } // end of if
        // increment the row counter
        counter ++;
    } // end of for
} // end of function

// function used to create or update a custom component that goes into a cell
juce::Component* PlaylistComponent::refreshComponentForCell (
    int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate
) {
    // add a custom play button to the third column for every row
    if (columnId == 2) {
        // check if the already exisiting component is a null pointer
        if (existingComponentToUpdate == nullptr) {
            // pointer to the play button
            juce::TextButton* btn = new juce::TextButton{"play"};
            // create and add an id to the button
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            // add an event listener to the button
            btn->addListener(this);
            // set existingComponentToUpdate to the button pointer
            existingComponentToUpdate = btn;
        }
    }
    
    // return the component pointer
    return existingComponentToUpdate;
} // end of function

// function called when a button is clicked
void PlaylistComponent::buttonClicked (juce::Button* btn) {
    // check if load button was clicked
    if (btn == &loadButton) {
        std::cout << "load button clicked" << std::endl;
        // open up file chooser
        juce::FileChooser chooser{"Select a file..."};
        // check if file chooser returns anything
        if (chooser.browseForMultipleFilesToOpen()) {
            auto results = chooser.getResults();
            // iterate over each choosen file
            for(auto& file: results) {
                if (formatManager->findFormatForFileExtension(file.getFileExtension())) {
                    // get the url for the file
                    juce::URL fileURL {file};
                    
                    // create a new pair that is then to be appended to the tracks
                    std::pair<std::string, juce::URL> newTrack;
                    newTrack.first = file.getFileName().toStdString();
                    newTrack.second = fileURL;
                    
                    // add the choosen files to the tracks map
                    tracks.insert(newTrack);
                    
                    // add the track to the data file
                    dataFile.appendText(fileURL.toString(false).toStdString() + "\r\n");
                }
                // update the contents of the table
                tableComponent.updateContent();
            } // end of for loop
        } // end of if
    } // end of if
    
    else {
        // get the of the button clicked from the button pointer
        int id = std::stoi(btn->getComponentID().toStdString());
        // implement the play button here
        
        // positive integer to keep track of the row number
        unsigned int counter = 0;
        // iterate over all the maps in tracks
        for (auto& item: tracks) {
            // if id is equal to row counter -- do somethings
            if (counter == id) {
                // load the track to the deck
                deck->loadURL(item.second);
                // break out of the loop
                break;
            }
            // increment the row counter
            counter ++;
        } // end of for
    } // end of else
} // end of function

// Callback to check whether this target is interested in the set
// of files being offered.
bool PlaylistComponent::isInterestedInFileDrag (const juce::StringArray &files) {
    return true;
}

// Callback to indicate that the user has dropped the
// files onto this component.
void PlaylistComponent::filesDropped (
   const juce::StringArray &files,
   int x,
   int y
) {
    // check if any files exist
    if (files.size() != 0) {
        // iterate over dropped files and add them to the table
        for (auto& item: files) {
            // create an instance of the file class
            juce::File file = juce::File{item};
            if (formatManager->findFormatForFileExtension(file.getFileExtension())) {
                // create a url variable for the file
                juce::URL fileURL{file};
                
                // create a new pair that is then to be appended to the tracks
                std::pair<std::string, juce::URL> newTrack;
                // add the name and the url of the file to the pair
                newTrack.first = file.getFileName().toStdString();
                newTrack.second = fileURL;
                // add the choosen files to the tracks map
                tracks.insert(newTrack);
                
                // add the track to the data file
                dataFile.appendText(fileURL.toString(false).toStdString() + "\r\n");
            }
            // update the contents of the table
            tableComponent.updateContent();
        } // end for
    } // end if
} // end function

// called when the user changes the text in the text editor
void PlaylistComponent::textEditorTextChanged  (juce::TextEditor& editor) {
    // check if everything in the editor is cleaned up
    if (editor.getText().toStdString() == ""){ // text in the text editor is empty
        // check if filtered track is empty
        if (filteredTracks.size() != 0) { // filtered track is not empty
            // iterate over all the tracks in the filtered tracks
            for (auto& t : filteredTracks) {
                // add each track to the track list
                tracks.insert(t);
            } // end for
            
            // clear all the tracks in the filtered list
            filteredTracks.clear();
        } // end if
    } // end if
    else { // text in the text editor is not empty
        // get the value in the editor
        std::string value = editor.getText().toStdString();
        // vector to store the track names
        std::vector<std::string> trackNames;
        // vector to store the track URLs
        std::vector<juce::URL> trackURLs;
        // iterate over all the tracks
        for (auto& t : tracks) {
            // get the track name
            std::string name = t.first;
            
            // check if the value in editor is in the name of the track
            if (name.find(value) != std::string::npos) {
                // push the track name and URL to their respective vectors
                trackNames.push_back(name);
                trackURLs.push_back(t.second);
            } // end if
            else { // value in editor not in the name of the track
                // insert the track to the filtered list
                filteredTracks.insert(t);
            } // end else
        } // end for
        
        // clear everything in tracks
        tracks.clear();
        // repopulate the tracks map with new values
        for (int i = 0; i < trackNames.size(); i++ ) {
            // create a new string and URL pair
            std::pair<std::string, juce::URL> newPair;
            // set the string value
            newPair.first = trackNames[i];
            // set the string url
            newPair.second = trackURLs[i];
            // insert the pair to the tracks
            tracks.insert(newPair);
        } // end for
    } // end else
    
    // update the content of the table
    tableComponent.updateContent();
} // end function
