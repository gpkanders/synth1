/*
  ==============================================================================

    synthVoice.cpp
    Created: 11 Mar 2024 2:28:27pm
    Author:  George Anders

  ==============================================================================
*/

#include "synthVoice.h"

bool synthVoice::canPlaySound(juce::SynthesiserSound* sound){
    return dynamic_cast<juce::SynthesiserSound*>(sound)!=nullptr;
}
void synthVoice::startNote(int midiNoteNumber,float velocity,juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    osc.setWaveFrequency(midiNoteNumber);
    adsr.noteOn();
}
void synthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
    
    if(!allowTailOff || !adsr.isActive()){
        clearCurrentNote();
    }
}
void synthVoice::controllerMoved(int controllernumber, int newControllerValue) {
    
}
void synthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels){
    
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec; 
    spec.maximumBlockSize=samplesPerBlock;
    spec.sampleRate=sampleRate;
    spec.numChannels=outputChannels;
    
    osc.prepareToPlay(spec); 
    gain.prepare(spec);
    
    
    gain.setGainLinear(0.3f);
    
    
    
    isPrepared=true;
}

void synthVoice::update(const float a, const float d, const float s, const float r){
    adsr.updateADSR(a,d,s,r);
}

void synthVoice::renderNextBlock(juce::AudioBuffer<float>&outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared);
    
    if(!isVoiceActive()){
        return;
    }
    
    synthBuffer.setSize(outputBuffer.getNumChannels(),numSamples,false,false,true); //avoidReallocating=true
    
    juce::dsp::AudioBlock<float> audioBlock {synthBuffer};
    osc.getNextAudioBlock(audioBlock);
    gain.process (juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    
    
    for(int channel=0;channel<outputBuffer.getNumChannels();++channel){
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if(!adsr.isActive()){
            clearCurrentNote();
        }
    }
    
}
void synthVoice::pitchWheelMoved(int newPitchWheelValue){
    
}
