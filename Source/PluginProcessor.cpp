/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SinesAudioProcessor::SinesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : easy::AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
  addFloatParameter("Gain", 0.0f, 1.0f, 1.0f);
}

SinesAudioProcessor::~SinesAudioProcessor()
{
}

//==============================================================================
const String SinesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SinesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SinesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SinesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SinesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SinesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SinesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SinesAudioProcessor::setCurrentProgram (int index)
{
}

const String SinesAudioProcessor::getProgramName (int index)
{
    return {};
}

void SinesAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SinesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SinesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SinesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SinesAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    const unsigned int buffer_size = buffer.getNumSamples();
    const float sampling_rate = (float)getSampleRate();
  
    float* left = buffer.getWritePointer(0);
    float* right = buffer.getWritePointer(1);
  
//    static int k = 0;
//    for (int channel = 0; channel < 1; ++channel)
//    {
//        float* channelData = buffer.getWritePointer (channel);
  
  
      float* outputs[] = {left, right};
        _sine.process(outputs, buffer_size);
  
//        k += buffer_size;

        // ..do something to the data...
//    }
}

//==============================================================================
bool SinesAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SinesAudioProcessor::createEditor()
{
    return new SinesAudioProcessorEditor (*this);
}

void SinesAudioProcessor::parameterChanged(const juce::String &id, float value) {
  if(id == "Gain") {
    // Do something with Gain value.
  } 
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SinesAudioProcessor();
}
