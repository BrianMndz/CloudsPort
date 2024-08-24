//
// Created by BrianMendoza on 16/08/2024.
//
#pragma once

#include "JuceHeader.h"
#include "../DSP/CloudsAudioBuffer.h"

enum class EnvelopeShape {
    Rectangular,
    Triangular,
    Hann
};

class Grain {
public:
    Grain() : active_(false), envelope_phase_(2.0f) {}

    void init();
    void start(int32_t buffer_size,
               int32_t start,
               int32_t length,
               float pitch_ratio,
               int32_t pre_delay,
               float pan,
               EnvelopeShape envelopeShape);
    void process(const CloudsAudioBuffer *buffer, float* out, size_t size);

    bool active() const { return active_; }

private:
    float interpolateBuffer(const CloudsAudioBuffer *buffer, float phase) const;
    float envelopeValue() const;

    bool active_;
    int32_t buffer_size_;
    int32_t start_;
    int32_t length_;
    float pitch_ratio_;
    int32_t pre_delay_;
    float pan_;
    EnvelopeShape envelope_shape_;

    float phase_;
    float envelope_phase_;
    float envelope_increment_;
};
