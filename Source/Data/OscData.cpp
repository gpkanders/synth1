/*
  ==============================================================================

    OscData.cpp
    Created: 22 Mar 2024 10:55:55am
    Author:  George Anders

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec){
    fmOsc.prepare(spec);
    prepare(spec);
    
}

void OscData::setWaveType(const int choice){
    // return...
    //   x/juce::MathConstants<float>::pi; Saw Wave
    //   x<0.0f ? -1.0f:1.0f; Square Wave
    // std::sin(x) sine wave
    
    switch (choice) {
        case 0:
            initialise([](float x) {return std::sin(x);});
            break;
        case 1:
            initialise([](float x) {return x/juce::MathConstants<float>::pi;});
            break;
        
        case 2:
            //square wave
            initialise([](float x){return x<0.0f ? -1.0f:1.0f;});
            break;
            
        default:
            jassertfalse; // you're not supposed to be here
            break;
    }
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block){
    
    for(int ch=0;ch<block.getNumChannels();++ch){
        for (int s=0; s<block.getNumSamples();++s){
            fmMod=fmOsc.processSample(block.getSample(ch,s))*fmDepth;
        }
    }
    
    process(juce::dsp::ProcessContextReplacing<float>(block));
    
}

void OscData::setWaveFrequency(const int midiNoteNumber){
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    lastMidiNote=midiNoteNumber;
}

void OscData::setFmParams(const float depth, const float freq){
    fmOsc.setFrequency(freq);
    fmDepth=depth;
    auto currentFreq=juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
    setFrequency(currentFreq>=0 ? currentFreq: currentFreq*1.0f); // returns abs if negative
}
