/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 6 Aug 2021 10:16:34pm
    Author:  Mohammad 

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager) {}

DJAudioPlayer::~DJAudioPlayer() {}

//==============================================================================
// tells the source to prepare for playing
void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // tell the transport source to prepare for playing
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // tell the resample sourece to prepare for playing
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// called repeatedly to fetch subsequent blocks of audio data
void DJAudioPlayer::getNextAudioBlock (
    const juce::AudioSourceChannelInfo& bufferToFill
) {
    // pass blocks of audio on to resample source
    resampleSource.getNextAudioBlock(bufferToFill);
}

// Allows source to release data that it does not need
void DJAudioPlayer::releaseResources()
{
    // tell the resample source to release all of its unwanted data
    resampleSource.releaseResources();
}

//==============================================================================
// load a track from a file system to the application
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    // create a pointer to a reader that can read the audio file
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    // if pointer is null the audio format is not supported
    if (reader != nullptr) { // audio format supported
        // create a unique pointer for the new source of the transport source
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (
            new juce::AudioFormatReaderSource (reader, true));
        
        // set the source of the transport sort
        transportSource.setSource(
                                  newSource.get(),
                                  0,
                                  nullptr,
                                  reader->sampleRate);
        
        // reset the reader source
        readerSource.reset(newSource.release());
    }
}

// Set the volume at which the audio is being played
void DJAudioPlayer::setGain(double gain)
{
    // check if gain is betwen 0 and 1
    if (gain < 0 || gain > 1.0) { // gain not between 0 and 1
        std::cout << "DJAudioPlayer::setGain  Gain should be between 0 and 1" << std::endl;
    }
    else { // gain is between 0 and 1
        // set the gain of the transport source to the
        // gain passed to the function
        transportSource.setGain(gain);
    }
}

// Sets the speed at which the audio is been played
void DJAudioPlayer::setSpeed(double ratio)
{
    // the speed ratio must be between 0 and 100
    if (ratio < 0 || ratio > 100.0) { // ratio is not between 0 and 100
        std::cout << "DJAudioPlayer::setGain  ratio should be between 0 and 100" << std::endl;
    }
    else { // ratio is between 0 and 100
        // set the speed of resampling source to ratio
        resampleSource.setResamplingRatio(ratio);
    }
}

// Set the position at which the audio is been played in seconds
void DJAudioPlayer::setPosition(double positionInSec)
{
    transportSource.setPosition(positionInSec);
}

// set the relative position of the audio being played
void DJAudioPlayer::setPositionRelative(double pos) {
    // relative position must be between 1 and 0
    if (pos < 0 || pos > 1.0) { // position is not between 1 and 0
        std::cout << "DJAudioPlayer::setGain  ratio should be between 0 and 1" << std::endl;
    }
    else { // position is between 0 and 1
        // convert the relative position to position in seconds
        double posInSec = transportSource.getLengthInSeconds() * pos;
        // call set position function with the converted value
        setPosition(posInSec);
    }
}

// Function called to start or play the audio
void DJAudioPlayer::start()
{
    transportSource.start();
}

// Function called to stop or pause the audio
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

// Function that returns the relative position
double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
