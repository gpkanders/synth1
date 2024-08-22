/*
  ==============================================================================

    synthSound.h
    Created: 11 Mar 2024 2:29:20pm
    Author:  George Anders

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class synthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override {return true;}
    bool appliesToChannel(int midiChannel) override {return true;}
};
