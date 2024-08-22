/*
  ==============================================================================

    AdsrData.h
    Created: 19 Mar 2024 6:14:26pm
    Author:  George Anders

  ==============================================================================
*/

#pragma once

#include<JuceHeader.h>

class AdsrData : public juce::ADSR{
public:
    void updateADSR(const float a, const float d, const float s, const float r);
    
private:
    juce::ADSR::Parameters adsrParams;
};
