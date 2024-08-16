#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CloudsAudioProcessor::CloudsAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    cloudsParams = std::make_unique<CloudsParameters>(*this);
}

CloudsAudioProcessor::~CloudsAudioProcessor()
{
}

//==============================================================================
const juce::String CloudsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CloudsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CloudsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CloudsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CloudsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CloudsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CloudsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CloudsAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String CloudsAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void CloudsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void CloudsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /** Testing audio buffer class*/
    inputBuffer.setSize(2, sampleRate * 10); // 10 seconds stereo buffer
    outputBuffer.setSize(2, sampleRate * 10);

    // Initialize wrapper, Testing audio buffer class*/
    inputBufferLeft.init(inputBuffer, 0);
    inputBufferRight.init(inputBuffer, 1);
    outputBufferLeft.init(outputBuffer, 0);
    outputBufferRight.init(outputBuffer, 1);

    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void CloudsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool CloudsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void CloudsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /** TESTING buffers */
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float leftSample = buffer.getSample(0, sample);
        float rightSample = buffer.getNumChannels() > 1 ? buffer.getSample(1, sample) : leftSample;

        // Write incoming audio to our input buffers
        inputBufferLeft.write(leftSample);
        inputBufferRight.write(rightSample);

        // Here you would call your ported Clouds processing code
        // using inputBufferLeft/Right for reading and outputBufferLeft/Right for writing

        // Read from output buffers and write to JUCE buffer
        buffer.setSample(0, sample, outputBufferLeft.read(0));
        if (buffer.getNumChannels() > 1)
            buffer.setSample(1, sample, outputBufferRight.read(0));
    }

    /* Testing buffer */
}

void CloudsAudioProcessor::updateParameters()
{
}

//==============================================================================
bool CloudsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CloudsAudioProcessor::createEditor()
{
    return new CloudsAudioProcessorEditor (*this);
}

//==============================================================================
void CloudsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void CloudsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CloudsAudioProcessor();
}
