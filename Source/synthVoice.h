/*
  ==============================================================================

    synthVoice.h
    Created: 11 Mar 2024 2:28:27pm
    Author:  George Anders

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "synthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"
#include "Data/FilterData.h"

class synthVoice: public juce::SynthesiserVoice{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber,float velocity,juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllernumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>&outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void update(const float a, const float d, const float s, const float r);
    OscData& getOscillator(){return osc;}

private:
    
    juce::AudioBuffer<float> synthBuffer;
    AdsrData adsr;
    
    OscData osc;
    //juce::dsp::Oscillator<float> osc {[](float x) {return x/juce::MathConstants<float>::pi;}};
    juce::dsp::Gain<float> gain;
    bool isPrepared{false};
    // return...
    //   x/juce::MathConstants<float>::pi; Saw Wave
    //   x<0.0f ? -1.0f:1.0f; Square Wave
    // std::sin(x) sine wave
    
};
