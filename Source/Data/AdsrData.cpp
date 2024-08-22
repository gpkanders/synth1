/*
  ==============================================================================

    AdsrData.cpp
    Created: 19 Mar 2024 6:14:26pm
    Author:  George Anders

  ==============================================================================
*/

#include "AdsrData.h"


void AdsrData::updateADSR(const float a, const float d, const float s, const float r){
    adsrParams.attack=a;
    adsrParams.decay=d;
    adsrParams.sustain=s;
    adsrParams.release=r;
    
    setParameters(adsrParams);
}
