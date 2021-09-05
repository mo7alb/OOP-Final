#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    // make the first deck gui visible
    addAndMakeVisible(deck1);
    // make the second deck gui visible
    addAndMakeVisible(deck2);
    
    // make the playlist component visible
    addAndMakeVisible(playlist);
    
    // let the format manager know about the basic audio fomats
    formatManager.registerBasicFormats();

    // set the background colors of all buttons to white
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    // set the text colors of all buttons to white
    getLookAndFeel().setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    // set the color of the label text to black
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::black);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
// Tells the source to prepare for playing 
void MainComponent::prepareToPlay (
    int samplesPerBlockExpected,
    double sampleRate
) {
    // let player 1 and 2 prepare to play the audio
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // let the mixer source know about player 1 and 2 to be able to mix audio
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

// Called repeatedly to fetch subsequent blocks of audio data
void MainComponent::getNextAudioBlock (
   const juce::AudioSourceChannelInfo& bufferToFill
) {
    mixerSource.getNextAudioBlock(bufferToFill);
}

// Allows the source to release anything it no longer needs after playback
// has stopped
void MainComponent::releaseResources()
{
    // let player 1 and 2 release their resources
    player1.releaseResources();
    player2.releaseResources();
    // let the mixer release its resources
    mixerSource.releaseResources();
}

//==============================================================================
// Called to draw component content
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

// Called when the component size has been changed 
void MainComponent::resized()
{
    // set bounds for the first deck GUI component
    deck1.setBounds(0, 0, getWidth()/2, getHeight()/1.6);
    // set bounds for the second deck GUI component
    deck2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/1.6);
    
    // set bound for the playlist component
    playlist.setBounds(0, getHeight()/1.6, getWidth(), getHeight()/2);
}
