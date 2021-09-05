#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    // pure virtual functions from AudioAppComponent
    /** Tells the source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** Called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    /** Allows the source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;

    // virtual Members of the component class
    /** Called to draw component content */
    void paint (juce::Graphics& g) override;
    /** Called when the component size has been changed */
    void resized() override;
    
private:
    // A manager that keeps a list of available audio formats and
    // decides which one to use to open a given file
    juce::AudioFormatManager formatManager;
    // used to manage multiple AudioThumbnail objects
    juce::AudioThumbnailCache thumbCache{20};
    
    // Audio player for the first deck
    DJAudioPlayer player1{formatManager};
    // GUI for the first deck
    DeckGUI deck1{&player1, formatManager, thumbCache};
    
    // Audio player for the second deck
    DJAudioPlayer player2{formatManager};
    // GUI for the second deck
    DeckGUI deck2{&player2, formatManager, thumbCache};
    
    // An audio source that allows mixing of multiple audio sources
    juce::MixerAudioSource mixerSource;
    
    // A custom component to display and use a playlist for multiple tracks
    PlaylistComponent playlist{&deck1, &formatManager};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
