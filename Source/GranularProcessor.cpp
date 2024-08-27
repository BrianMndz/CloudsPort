//
// Created by BrianMendoza on 16/08/2024.
//

#include "GranularProcessor.h"

void GranularProcessor::init(float sampleRate, int bufferSize)
{
    sample_rate_ = sampleRate;
    buffer_size_ = bufferSize;

    float freezeTimeSecs = 5.0f;
    for (size_t i = 0; i < 2; ++i)
    {
        input_buffers_[i].setSize(1, bufferSize);
        output_buffers_[i].setSize(1, bufferSize);
        freeze_buffers_[i].setSize(1, static_cast<int>(sampleRate * freezeTimeSecs));
        buffer_[i].init(input_buffers_[i], freeze_buffers_[i], 0, sampleRate, freezeTimeSecs);
    }

    playback_mode_ = PLAYBACK_MODE_GRANULAR;
    num_channels_ = 2;

    initDSPObjects();
}

void GranularProcessor::process(const float* const* inputs, float* const* outputs, int num_frames)
{
    updateGranularParameters();

    //Write input buffer
    for (size_t c = 0; c < num_channels_; ++c) {
        for (size_t i = 0; i < num_frames; ++i) {
            buffer_[c].write(inputs[c][i]);
        }
    }

    for (size_t c = 0; c < num_channels_; ++c)
        output_buffers_[c].clear();


    switch (playback_mode_)
    {
        case PLAYBACK_MODE_GRANULAR:
            processGranular(num_frames);
            break;
        default:
            break;
    }

    // Copy processed audio to output
    for (int c = 0; c < num_channels_; ++c)
        for (int i = 0; i < num_frames; ++i)
            outputs[c][i] = output_buffers_[c].getSample(0, i) * master_gain_;

}

void GranularProcessor::initDSPObjects()
{
    grainPlayer_.init(num_channels_, MAX_NUM_GRAINS);
}

void GranularProcessor::processGranular(int num_frames)
{
    std::vector<float> temp_buffer(num_frames * 2);
    grainPlayer_.process(buffer_, parameters_, temp_buffer.data(), num_frames);

    for (int c = 0; c < num_channels_; ++c)
    {
        for (size_t i = 0; i < num_frames; ++i)
        {
            output_buffers_[c].setSample(0, i, temp_buffer[i * 2 + c]);
        }
    }

    // Normalize output
    float max_sample = 0.0f;
    for (int c = 0; c < num_channels_; ++c)
    {
        for (int i = 0; i < num_frames; ++i)
        {
            max_sample = std::max(max_sample, std::abs(output_buffers_[c].getSample(0, i)));
        }
    }

    if (max_sample > 1.0f)
    {
        float normalize_factor = 1.0f / max_sample;
        for (int c = 0; c < num_channels_; ++c)
        {
            for (int i = 0; i < num_frames; ++i)
            {
                float sample = output_buffers_[c].getSample(0, i);
                output_buffers_[c].setSample(0, i, sample * normalize_factor);
            }
        }
    }
}

void GranularProcessor::updateGranularParameters()
{
    if(parameters_)
    {
        auto& apvts = parameters_->getAPVTS();
        grainPlayer_.setDensity(apvts.getRawParameterValue("density")->load());
        grainPlayer_.setStereoSpread(apvts.getRawParameterValue("stereo_spread")->load());
    }
}

void GranularProcessor::setFreeze(bool freeze)
{
    is_frozen_ = freeze;
    for (size_t i = 0; i < num_channels_; ++i)
    {
        buffer_[i].setFrozen(freeze);
    }
}


