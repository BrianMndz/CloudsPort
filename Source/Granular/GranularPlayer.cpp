//
// Created by BrianMendoza on 16/08/2024.
//

#include "GranularPlayer.h"

void GranularPlayer::init(int numChannels, int maxNumGrains)
{
    num_channels_ = numChannels;
    max_num_grains_ = maxNumGrains;
    num_grains_ = 0.0f;
    grain_size_hint_ = 1024.f;

    grains_.clear();
    grains_.resize(max_num_grains_);
    envelope_buffer_.resize(2048);  // Adjust size as needed

    for (auto& grain : grains_)
        grain.init();
}

void GranularPlayer::process(const CloudsAudioBuffer* buffer, const CloudsParameters& parameters, float* out, size_t size)
{

}

