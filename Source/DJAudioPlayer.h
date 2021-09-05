/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 6 Aug 2021 10:16:34pm
    Author:  Mohammad 

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>


class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    /** Tells the source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** Called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    /** Allows the source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;
    
    //==============================================================================
    /** function to load a track into the application */
    void loadURL(juce::URL audioUrl);
    /** Set the gain or the volume at which the audio is playing */
    void setGain(double gain);
    /** Sets the speed at which the audio plays */
    void setSpeed(double ratio);
    /** Sets the position of the audio in seconds */
    void setPosition(double positionInSec);
    /** Sets the relative position of the player */
    void setPositionRelative(double pos);
    /** Starts the audio */
    void start();
    /** Stops the audio */
    void stop();
    /** Get the relative position of the playhead */
    double getPositionRelative();
    
private:
    // A manager that keeps a list of available audio formats and
    // decides which one to use to open a given file
    juce::AudioFormatManager& formatManager;
    
    // A unique pointer to a type of AudioSource that will read
    // from an AudioFormatReader
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    
    // An audio source that takes a track and allows it to be played, stopped etc.
    juce::AudioTransportSource transportSource;
    
    // A type of AudioSource that takes an input source and changes its sample rate
    juce::ResamplingAudioSource resampleSource {&transportSource, false, 2};
};
