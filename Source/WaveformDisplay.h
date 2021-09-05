/*
  ==============================================================================

    WaveformDisplay.h
    Created: 7 Aug 2021 8:19:13pm
    Author:  Mohammad 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay
    : public juce::Component,
    public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
                    
    ~WaveformDisplay() override;
    
    /** Called to draw the content of the component */
    void paint (juce::Graphics&) override;
    /** Called when the component is resized */
    void resized() override;

    // Callback called when recieves change events 
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    
    /** Load the audio file into the audio thumbnail */
    void loadURL (juce::URL audioURL);
    
    /** Set the relative position of the playhead */
    void setPositionRelative(double position);
private:
    // Allows to draw a waveform
    juce::AudioThumbnail audioThumb;
    
    // boolean variable to check if the file
    // is loaded or not
    bool fileLoaded;
    
    // a double to keep track of the position 
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
