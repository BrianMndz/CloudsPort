#pragma once

#include "Helpers/Parameters.h"
#include "DSP/CloudsAudioBuffer.h"

#include <JuceHeader.h>

//==============================================================================
class CloudsAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    CloudsAudioProcessor();
    ~CloudsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    std::unique_ptr<CloudsParameters> cloudsParams;
    void updateParameters();

    /** Testing audio buffer class*/
    CloudsAudioBuffer inputBufferLeft;
    CloudsAudioBuffer inputBufferRight;
    CloudsAudioBuffer outputBufferLeft;
    CloudsAudioBuffer outputBufferRight;

private:
    /** Testing audio buffer class*/
    juce::AudioBuffer<float> inputBuffer;
    juce::AudioBuffer<float> outputBuffer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudsAudioProcessor)
};
