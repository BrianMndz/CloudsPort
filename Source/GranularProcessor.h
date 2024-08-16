//
// Created by BrianMendoza on 16/08/2024.
//
#pragma once

#include "JuceHeader.h"
#include "DSP/CloudsAudioBuffer.h"
#include "Helpers/Parameters.h"

enum PlaybackMode
{
    PLAYBACK_MODE_GRANULAR,
    PLAYBACK_MODE_STRETCH,
    PLAYBACK_MODE_LOOPING_DELAY,
    PLAYBACK_MODE_SPECTRAL,
};

class GranularProcessor {
public:
    GranularProcessor() = default;

    void init(float sampleRate, int bufferSize);
    void process(const float* const *inputs, float * const* outputs, int num_frames);

    void setParameters(CloudsParameters* params) { parameters_ = params; }
    void setPlaybackMode(const PlaybackMode mode) { playback_mode_ = mode; }

private:
    void initDSPObjects();
    void processGranular(int num_frames);

    float sample_rate_{};
    int buffer_size_{};
    int num_channels_{};
    PlaybackMode playback_mode_;

    juce::AudioBuffer<float> input_buffers_[2];
    juce::AudioBuffer<float> output_buffers_[2];
    CloudsAudioBuffer buffer_[2];

    CloudsParameters* parameters_;

    // Add other necessary member variables and objects
    // (e.g., GranularSamplePlayer player_, WSOLASamplePlayer ws_player_, etc.)
};
