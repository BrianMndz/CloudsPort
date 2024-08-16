//
// Created by BrianMendoza on 16/08/2024.
//

#include "GranularProcessor.h"

void GranularProcessor::init(float sampleRate, int bufferSize)
{
    sample_rate_ = sampleRate;
    buffer_size_ = bufferSize;

    for (size_t i = 0; i < 2; ++i)
    {
        input_buffers_[i].setSize(1, bufferSize);
        output_buffers_[i].setSize(1, bufferSize);
        buffer_[i].init(input_buffers_[i], 0);
    }

    playback_mode_ = PLAYBACK_MODE_GRANULAR;
    num_channels_ = 2;

    initDSPObjects();
}

void GranularProcessor::process(const float* const* inputs, float* const* outputs, int num_frames)
{
    //Write input buffer
    for (size_t c = 0; c < num_channels_; ++c)
    {
        for (size_t i = 0; i < num_frames; ++i)
        {
            buffer_[c].write(inputs[c][i]);
        }
    }

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
    {
        for (int i = 0; i < num_frames; ++i)
        {
            outputs[c][i] = output_buffers_[c].getSample(0, i);
        }
    }
}

void GranularProcessor::initDSPObjects()
{
}

void GranularProcessor::processGranular(int num_frames)
{
}


