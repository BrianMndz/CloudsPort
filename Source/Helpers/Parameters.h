//
// Created by BrianMendoza on 16/08/2024.
//
#pragma once

#include "JuceHeader.h"

class CloudParameters
{
public:
    explicit CloudParameters(juce::AudioProcessor &processor)
    {
        auto parametersLayout = createParamaterLayout();
        apvts = std::make_unique<juce::AudioProcessorValueTreeState>(processor, nullptr, "Parameters", std::move(parametersLayout));
    }

    juce::AudioProcessorValueTreeState& getAPVTS() { return  *apvts; }

private:
    std::unique_ptr<juce::AudioProcessorValueTreeState> apvts;

    juce::AudioProcessorValueTreeState::ParameterLayout createParamaterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        layout.add(std::make_unique<juce::AudioParameterFloat>("position", "Position", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("size", "Size", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("pitch", "Pitch", -24.0f, 24.0f, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("density", "Density", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("texture", "Texture", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("dry_wet", "Dry/Wet", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("stereo_spread", "Stereo Spread", 0.0f, 1.0f, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.0f, 1.0f, 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("reverb", "Reverb", 0.0f, 1.0f, 0.0f));

        layout.add(std::make_unique<juce::AudioParameterBool>("freeze", "Freeze", false));
        layout.add(std::make_unique<juce::AudioParameterBool>("trigger", "Trigger", false));
        layout.add(std::make_unique<juce::AudioParameterBool>("gate", "Gate", false));

        return layout;
    }
};