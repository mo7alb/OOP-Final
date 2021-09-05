/*
  ==============================================================================

    DeckGUI.h
    Created: 7 Aug 2021 1:02:30pm
    Author:  Mohammad 

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 the GUI for the the deck that has some components in it. it basically has some
 buttons and sliders
*/
class DeckGUI
    : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    /** Called to draw component content */
    void paint (juce::Graphics&) override;
    /** Called when the component size has been changed */
    void resized() override;

    /** function called when a button is clicked  */
    void buttonClicked (juce::Button *) override;
    
    /** function called when the slider value changes  */
    void sliderValueChanged (juce::Slider* slider) override;
    
    /** Callback to check whether this target is interested in the set of files being offered. */
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    /** Callback to indicate that the user has dropped the files onto this component. */
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    
    /** User defined callback that gets called periodically */
    void timerCallback () override;
    
    /*
     Function to add the audio file to the player and the
     waveform display component
    */
    void loadURL(juce::URL url);
    
private:
    // private members go here
    
    // play button
    juce::TextButton playButton{"PLAY"};
    // stop button
    juce::TextButton stopButton{"STOP"};
    // Load button
    juce::TextButton loadButton{"LOAD"};
    
    // label for volume slider
    juce::Label volumeLabel;
    // volume slider
    juce::Slider volumeSlider{};
    
    // label for volume slider
    juce::Label speedLabel;
    // speed slider
    juce::Slider speedSlider{};
    
    // label for volume slider
    juce::Label posLabel;
    // position slider
    juce::Slider posSlider{};
    
    // audio player
    DJAudioPlayer* player;
    
    // implement the WaveformDisplay component in the DeckGUI component
    WaveformDisplay waveformDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
