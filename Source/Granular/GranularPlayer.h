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
                const CloudsParameters& parameters,
                float* out,
                size_t size);

private:
    void scheduleGrain(Grain* grain,
                const CloudsParameters& parameters,
                int32_t pre_delay,
                int32_t buffer_size,
                int32_t buffer_head);
    float interpolateParameter(float start, float end, float phase);

    int num_channels_ = 0;
    int max_num_grains_ = 0;
    float num_grains_ = 0;
    float grain_size_hint_ = 1024.f;

    std::vector<Grain> grains_;
    std::vector<float> envelope_buffer_;
    std::mt19937 random_generator_{std::random_device()()};
    std::uniform_real_distribution<float> distribution_{0.0f, 1.0f};
};

