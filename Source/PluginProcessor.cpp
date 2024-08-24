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
    granularProcessor = std::make_unique<GranularProcessor>();
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
    granularProcessor->init(static_cast<float>(sampleRate), samplesPerBlock);
    granularProcessor->setParameters(cloudsParams.get());
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

    // Get pointers to input and output channel data
    auto inputChannelData = buffer.getArrayOfReadPointers();
    auto outputChannelData = buffer.getArrayOfWritePointers();

    granularProcessor->process(inputChannelData, outputChannelData, buffer.getNumSamples());

    // Update parameters
    updateParameters();
}

void CloudsAudioProcessor::updateParameters()
{
    granularProcessor->setParameters(cloudsParams.get());
    float master_volume = cloudsParams->getAPVTS().getRawParameterValue("master_volume")->load();
    granularProcessor->setMasterGain(master_volume);
}

//==============================================================================
bool CloudsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CloudsAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
    //return new CloudsAudioProcessorEditor (*this);
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
