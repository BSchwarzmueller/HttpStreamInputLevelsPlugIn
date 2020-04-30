/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "HttpStream.h"

//==============================================================================
HttpStreamInputLevelsAudioProcessor::HttpStreamInputLevelsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       //.withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withInput  ("Input",  AudioChannelSet::discreteChannels(NUMBER_OF_CHANNELS), true)
                      #endif
                       //.withOutput ("Output", AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::discreteChannels(NUMBER_OF_CHANNELS), true)
                     #endif
                       )
#endif
{
}

HttpStreamInputLevelsAudioProcessor::~HttpStreamInputLevelsAudioProcessor()
{
}

//==============================================================================
const String HttpStreamInputLevelsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HttpStreamInputLevelsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HttpStreamInputLevelsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HttpStreamInputLevelsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HttpStreamInputLevelsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HttpStreamInputLevelsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HttpStreamInputLevelsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HttpStreamInputLevelsAudioProcessor::setCurrentProgram (int index)
{
}

const String HttpStreamInputLevelsAudioProcessor::getProgramName (int index)
{
    return {};
}

void HttpStreamInputLevelsAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HttpStreamInputLevelsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void HttpStreamInputLevelsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HttpStreamInputLevelsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    /*
    if (layouts.getMainInputChannelSet() != AudioChannelSet::discreteChannels(NUMBER_OF_CHANNELS)
     && layouts.getMainOutputChannelSet() != AudioChannelSet::discreteChannels(NUMBER_OF_CHANNELS))
        return false; */

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HttpStreamInputLevelsAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;  //?
    
    int currentBufferSize    = buffer.getNumSamples();
    double currentSampleRate = getSampleRate();
    
    if(!StreamLevelInputs.getBufferSize())  // Um die Samples-per-Buffer-Rate zu berechnen werden die Werte SampleRate und Größe des Buffers benötigt
    {
        StreamLevelInputs.setSampleRate(currentSampleRate);
        StreamLevelInputs.setBufferSize(currentBufferSize);
    }
    
    float tmpRMSLevel;
    
    for (int channel = 0; channel < NUMBER_OF_CHANNELS; ++channel)
    {
        tmpRMSLevel = buffer.getRMSLevel(channel, 0, currentBufferSize);
        StreamLevelInputs.getRMSLevelPerChannel(channel, tmpRMSLevel);
    }
}

//==============================================================================
bool HttpStreamInputLevelsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HttpStreamInputLevelsAudioProcessor::createEditor()
{
    return new HttpStreamInputLevelsAudioProcessorEditor (*this);
}

//==============================================================================
void HttpStreamInputLevelsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HttpStreamInputLevelsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HttpStreamInputLevelsAudioProcessor();
}
