/*
  ==============================================================================

    DeckGUI.cpp
    Created: 7 Aug 2021 1:02:30pm
    Author:  Mohammad 

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse
) : player(_player), // initialize player
    waveformDisplay(formatManagerToUse, cacheToUse) // initialize waveform display component
{
    // make the play button component visible to the screen
    addAndMakeVisible(playButton);
    // make the stop button component visible to the screen
    addAndMakeVisible(stopButton);
    // make the Load button component visible to the screen
    addAndMakeVisible(loadButton);
    // add and make the volume label visible
    addAndMakeVisible(volumeLabel);
    // make the volume slider component visible to the screen
    addAndMakeVisible(volumeSlider);
    // add and make the volume label visible
    addAndMakeVisible(speedLabel);
    // make the speed slider component visible to the screen
    addAndMakeVisible(speedSlider);
    // add and make the volume label visible
    addAndMakeVisible(posLabel);
    // make the position slider component visible to the screen
    addAndMakeVisible(posSlider);
    // add and make visible the waveform display component
    addAndMakeVisible(waveformDisplay);
    startTimer(500);
    
    // add a button event listener to the play button
    playButton.addListener(this);
    // add a button event listener to the stop button
    stopButton.addListener(this);
    // add a button event listener to the load button
    loadButton.addListener(this);
    // add a slider event listener to the volume slider
    volumeSlider.addListener(this);
    // add a slider event listener to the speed slider
    speedSlider.addListener(this);
    // add a slider event listener to the position slider
    posSlider.addListener(this);
  
    // set the text of the volume slider
    volumeLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    // set the text of the volume slider
    speedLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    // set the text of the volume slider
    posLabel.setText("Position", juce::NotificationType::dontSendNotification);
    
    

    // set value box styles for all sliders
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    
    // set the range of the volume slider from 0 to 1
    volumeSlider.setRange(0.0, 1.0);
    // set the range of the speed slider from 1 to 10
    speedSlider.setRange(1.0, 10.0);
    // set the range of the position slider from 0 to 1
    posSlider.setRange(0.0, 1.0);
    
    // set thumb color
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::black);
    // set Slider covered area to white color
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::white);
    // set the background of the slider to dark grey
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
    
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

// called to draw the component content
void DeckGUI::paint (juce::Graphics& g)
{
    // set background color
    g.fillAll (juce::Colours::grey);
    
    // draw a reactangle all around the component
    g.setColour(juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);
}


// called when the component size changes
void DeckGUI::resized()
{
    // divide the screen into 8 parts
    double rowH = getHeight()/8;
    
    // set the x, y, width and height of the play button
    playButton.setBounds(10, 10, getWidth()/2 - 11, rowH - 10);
    // set the x, y, width and height of the stop button
    stopButton.setBounds(getWidth()/2 + 1, 10, getWidth()/2 - 10, rowH - 10);
    
    // set the x, y, width and height of the volume label
    volumeLabel.setBounds(2, rowH, getWidth(), rowH);
    // set the x, y, width and height of the volume slider
    volumeSlider.setBounds(getWidth() / 7, rowH, getWidth() - 75, rowH);
    
    // set the x, y, width and height of the speed slider
    speedLabel.setBounds(2, rowH * 2, getWidth(), rowH);
    // set the x, y, width and height of the speed slider
    speedSlider.setBounds(getWidth() / 7, rowH * 2, getWidth() - 75, rowH);
    
    // set the x, y, width and height of the speed slider
    posLabel.setBounds(2, rowH * 3, getWidth(), rowH);
    // set the x, y, width and height of the speed slider
    posSlider.setBounds(getWidth() / 7, rowH * 3, getWidth() - 75, rowH);
    
    // set the x, y, width and height of the waveform display component
    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 3);
    
    // set the x, y, width and height of the load button
    loadButton.setBounds(10, rowH * 7 + 10, getWidth() - 20, rowH - 10);
}

// Button event listener
void DeckGUI::buttonClicked(juce::Button* button) {
    // check if the clicked button pointer passed has the same
    // address as the playbutton
    if (button == &playButton) {
        player->start();
    }
    // check if button pointer passed has the same
    // address as the stopButton
    if (button == &stopButton) {
        player->stop();
    }
    // check if the clicked button pointer passed has the same
    // address as the loadButton
    if (button == &loadButton) {
        // open file selector
        juce::FileChooser chooser{"Select a file..."};
        // check if a file is choosen
        if (chooser.browseForFileToOpen()) { // file is choosen
            juce::File file = chooser.getResult();
            loadURL(juce::URL{file});
        }
    }
}

// slider event listener
void DeckGUI::sliderValueChanged (juce::Slider* slider) {
    // check if the slider pointer passed has the same
    // address as the volumeSlider
    if (slider == &volumeSlider) {
        player->setGain(slider->getValue());
    }
    // check if the slider pointer passed has the same
    // address as the speedSlider
    if (slider == &speedSlider) {
        // set the speed of the audio the value of the speedSlider
        player->setSpeed(slider->getValue());
    }
    // check if the slider pointer passed has the same
    // address as the posSlider
    if (slider == &posSlider) {
        // set the position in seconds of the audio the value of the posSlider
        player->setPositionRelative(slider->getValue());
    }
}

// check whether this target is interested in the set of files being offered
bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files) {
    return true;
}

// Callback to indicate that the user has dropped the files onto this component
void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y) {
    // check if a single file is dropped on the component
    if (files.size() == 1) { // single file droped on component
        // call loadURL function on the file
        loadURL(juce::URL{files[0]});
    }
}

// a callback that gets called periodically
void DeckGUI::timerCallback () {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

// function to load a file into the player and wave form display
void DeckGUI::loadURL(juce::URL url) {
    // load the file in the audio player
    player->loadURL(url);
    // load the file into the waveformdispaly component
    waveformDisplay.loadURL(url);
}
