//
// Created by BrianMendoza on 16/08/2024.
//

#include "Grain.h"

void Grain::init()
{
    active_ = false;
    envelope_phase_ = 2.0f;
}

void Grain::start(int32_t buffer_size, int32_t start, int32_t length, float pitch_ratio, int32_t pre_delay)
{
    buffer_size_ = buffer_size;
    start_ = start;
    length_ = length;
    pitch_ratio_ = pitch_ratio;
    pre_delay_ = pre_delay;

    phase_ = 0.0f;
    envelope_phase_ = 0.0f;
    envelope_increment_ = 2.0f / static_cast<float>(length);

    active_ = true;
}

void Grain::process(const CloudsAudioBuffer* buffer, float* out, size_t size)
{
    if (!active_) return;

    for (size_t i = 0; i < size; ++i)
    {
        if (pre_delay_ > 0)
        {
            --pre_delay_;
            out[i * 2] = out[i * 2 + 1] = 0.0f;
            continue;
        }

        float sample = interpolateBuffer(buffer, phase_);
        float envelope = envelopeValue();

        out[i * 2] = out[i * 2 + 1] = sample * envelope;

        phase_ += pitch_ratio_;
        if (phase_ >= static_cast<float>(length_))
        {
            active_ = false;
            break;
        }

        envelope_phase_ += envelope_increment_;
        if (envelope_phase_ >= 2.0f)
        {
            active_ = false;
            break;
        }
    }
}

float Grain::interpolateBuffer(const CloudsAudioBuffer* buffer, float phase) const
{
    int32_t index = (start_ + static_cast<int32_t>(phase)) % buffer_size_;
    float fraction = phase - std::floor(phase);

    float sample1 = buffer->read(index);
    float sample2 = buffer->read((index + 1) % buffer_size_);

    return sample1 + fraction * (sample2 - sample1);
}

float Grain::envelopeValue() const
{
    if (envelope_phase_ < 1.0f)
        return envelope_phase_;
    else
        return 2.0f - envelope_phase_;
}



