/*
  ==============================================================================

    FilterData.cpp
    Created: 30 May 2024 6:44:05pm
    Author:  George Anders

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels){
    filter.reset();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    
    filter.prepare(spec);
    isPrepared=true;
    
}


void FilterData::process(juce::AudioBuffer<float>& buffer){
    jassert(isPrepared);
    
    juce::dsp::AudioBlock<float> block {buffer};
    
    filter.process(juce::dsp::ProcessContextReplacing<float> {block});
}


void FilterData::updateParameters(const int filterType, const float frequency, const float resonance){
    switch (filterType) {
        case 0:
            filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
            break;
        case 1:
            filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
            break;
        case 2:
            filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
            break;
        default:
            break;
    }
    
    filter.setCutoffFrequency(frequency);
    filter.setResonance(resonance);
    
}


void reset(){
    
}
