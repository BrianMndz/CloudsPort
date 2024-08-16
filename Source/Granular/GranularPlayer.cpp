//
// Created by BrianMendoza on 16/08/2024.
//

#include "GranularPlayer.h"

void GranularPlayer::init(int numChannels, int maxNumGrains)
{
    num_channels_ = numChannels;
    max_num_grains_ = maxNumGrains;
    grains_.resize(maxNumGrains);

    for (auto& grain : grains_)
        grain.init();
}

void GranularPlayer::process(const CloudsAudioBuffer* buffer, CloudsParameters* parameters, float* out, size_t size)
{
    float overlap = parameters->getAPVTS().getRawParameterValue("density")->load();
    overlap = overlap * overlap * overlap;
    float target_num_grains = max_num_grains_ * overlap;

    std::fill(out, out + size * 2, 0.0f);

    for (auto& grain : grains_)
    {
        if (grain.active())
        {
            grain.process(buffer, out, size);
        }
    }

    //Schedule new grains
    float grain_probability = target_num_grains / grain_size_hint_;
    for (size_t i = 0; i < size; ++i)
    {
        if (juce::Random::getSystemRandom().nextFloat() < grain_probability)
        {
            scheduleGrain(parameters, buffer->size(), buffer->head() - size + i);
        }
    }

    //Overall gain
    float gain = 1.0f / std::sqrt(num_grains_);
    for (size_t i = 0; i < size; ++i)
    {
        out[i] *= gain;
    }
}

void GranularPlayer::scheduleGrain(CloudsParameters* parameters, int32_t buffer_size, int32_t buffer_head)
{
    auto it = std::find_if(grains_.begin(), grains_.end(), [](const Grain& g){
        return !g.active(); });
    if (it == grains_.end()) return;

    float position = parameters->getAPVTS().getRawParameterValue("position")->load();
    float size = parameters->getAPVTS().getRawParameterValue("size")->load();
    float pitch = parameters->getAPVTS().getRawParameterValue("pitch")->load();

    int32_t grain_size = static_cast<int32_t>(size * 100000.f);
    int32_t start = static_cast<int32_t>(position * buffer_size);
    float pitch_ratio = std::pow(2.0f, pitch / 12.0f);

    it->start(buffer_size, start, grain_size, pitch_ratio, 0);

    num_grains_ = std::min(num_grains_ + 1.0f, static_cast<float>(max_num_grains_));
}
