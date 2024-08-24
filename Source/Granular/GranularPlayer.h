//
// Created by BrianMendoza on 16/08/2024.
//
#pragma once

#include "JuceHeader.h"
#include "../DSP/CloudsAudioBuffer.h"
#include "../Helpers/Parameters.h"
#include "Grain.h"
#include <random>

class GranularPlayer {
public:
    GranularPlayer() {}

    void init(int numChannels, int maxNumGrains);
    void process(const CloudsAudioBuffer* buffer,
                CloudsParameters* parameters,
                float* out,
                size_t size);

    void setDensity(float density) { density_ = density; }
    void setStereoSpread(float spread) { stereo_spread_ = spread; }
    void setEnvelopeShape(EnvelopeShape shape) { envelope_shape_ = shape; }

    void setPosition(float position);   //Not implemented yet
    void setSize(float size);           //Not implemented yet
    void setPitch(float pitch);         //Not implemented yet

private:
    void scheduleGrain(CloudsParameters* parameters,
                int32_t buffer_size,
                int32_t buffer_head);
    float calculateGrainPan();
    float getEnvelopeValue(float phase);

    float density_ = 0.5f;
    float stereo_spread_ = 0.0f;
    EnvelopeShape envelope_shape_;

    int32_t num_channels_ = 0;
    int32_t max_num_grains_ = 0;
    float num_grains_ = 0.0f;
    float grain_size_hint_ = 1024.f;

    std::vector<Grain> grains_;
    std::mt19937 rng_{std::random_device{}()};
};

