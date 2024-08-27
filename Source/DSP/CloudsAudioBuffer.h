//
// Created by BrianMendoza on 16/08/2024.
//
#pragma once

#include "JuceHeader.h"

class CloudsAudioBuffer
{
public:
    CloudsAudioBuffer() : size_(0), write_head_(0) {}

    void init(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& frBuffer, int channel, float sampleRate, float freezeTimeSecs)
    {
        buffer_ = &buffer;
        freeze_buffer_ = &frBuffer;
        channel_ = channel;
        size_ = buffer.getNumSamples();
        write_head_ = 0;

        freeze_buffer_size_ = static_cast<int>(sampleRate * freezeTimeSecs);
        freeze_buffer_->setSize(1, freeze_buffer_size_);
        freeze_write_head_ = 0;
    }

    void write(float in)
    {
        buffer_->setSample(channel_, write_head_, in);
        write_head_ = (write_head_ + 1) % size_;

        freeze_buffer_->setSample(0, freeze_write_head_, in);
        freeze_write_head_ = (freeze_write_head_ + 1) % freeze_buffer_size_;
    }

    float read(int32_t offset) const
    {
        if (is_frozen_)
        {
            int32_t index = (freeze_write_head_ - offset + freeze_buffer_size_) % freeze_buffer_size_;
            return freeze_buffer_->getSample(0, index);
        }
        else
        {
            int32_t index = (write_head_ - offset + size_) % size_;
            return buffer_->getSample(channel_, index);
        }
    }

    float readHermite(int32_t offset, uint16_t fractional) const
    {
        float t = fractional / 65536.0f;
        int32_t index = (write_head_ - offset + size_) % size_;

        float xm1 = read(offset + 1);
        float x0 = read(offset);
        float x1 = read(offset - 1);
        float x2 = read(offset - 2);

        float c = (x1 - xm1) * 0.5f;
        float v = x0 - x1;
        float w = c + v;
        float a = w + v + (x2 - x0) * 0.5f;
        float b_neg = w + a;

        return ((((a * t) - b_neg) * t + c) * t + x0);
    }

    int32_t size() const { return size_; }
    int32_t head() const { return write_head_; }
    void setFrozen(bool frozen) { is_frozen_ = frozen; }

private:
    juce::AudioBuffer<float> *buffer_;
    int channel_;
    int32_t size_;
    int32_t write_head_;

    juce::AudioBuffer<float> *freeze_buffer_;
    int32_t freeze_buffer_size_;
    int32_t freeze_write_head_;
    bool is_frozen_ = false;
};
