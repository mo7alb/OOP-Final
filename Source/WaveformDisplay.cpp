/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 7 Aug 2021 8:19:13pm
    Author:  Mohammad 

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
                                 juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse
) :
    audioThumb(1000, formatManagerToUse, cacheToUse),
    position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

// called to draw the content of the component
void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::white);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    // check if the audio is loaded
    // if audio is loaded set draw the waveform
    if (fileLoaded) {
        // set the color of the waveform
        g.setColour (juce::Colours::darkgrey);
        // draw the waveform
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f);
        
        // set color for the playhead
        g.setColour(juce::Colours::black);
        // draw the playhead as a rectangle
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight(), 2);
    }
    // if audio file did not load print message
    else {
        // set the color for the text
        g.setColour(juce::Colours::black);
        // set the font size for the message
        g.setFont (20.0f);
        // print message
        g.drawText ("File not loaded...",
                    getLocalBounds(),
                    juce::Justification::centred,
                    true);   // draw some placeholder text
    }
}

// callad when the component sizes change
void WaveformDisplay::resized()
{
}

// function to load the audio file into the thumb nail
void WaveformDisplay::loadURL (juce::URL audioURL) {
    // clear any previous thumb nail
    audioThumb.clear();
    
    // set the fileLoaded the value of setting the source of the audio thumb the file
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    
    if (fileLoaded) { // check if file loaded
        std::cout << "WFD: loaded!" << fileLoaded << std::endl;
    }
    else { // file not loaded
        std::cout << "WFD: not Loaded!" << std::endl;
    }
}

// Repaint the whole component if any changes occur
void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster* source) {
    repaint();
}

// Called periodically to set the position of the playhead
void WaveformDisplay::setPositionRelative(double pos) {
    // check if the position saved is the same as the new position
    if (position != pos) { // position not the same
        // change the position
        position = pos;
        // repaint the component
        repaint();
    }
}
