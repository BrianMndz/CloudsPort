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

private:
    void scheduleGrain(CloudsParameters* parameters,
                int32_t buffer_size,
                int32_t buffer_head);

    int32_t num_channels_ = 0;
    int32_t max_num_grains_ = 0;
    float num_grains_ = 0.0f;
    float grain_size_hint_ = 1024.f;

    std::vector<Grain> grains_;
};

