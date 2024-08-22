/*
  ==============================================================================

    FilterData.h
    Created: 30 May 2024 6:44:05pm
    Author:  George Anders

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float frequency, const float resonance);
    void reset();
    
private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared;
};
