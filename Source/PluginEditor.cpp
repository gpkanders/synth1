/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Synth1AudioProcessorEditor::Synth1AudioProcessorEditor (Synth1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc (audioProcessor.apvts,"OSC1WAVETYPE", "FMFREQ","FMDEPTH"), adsr(audioProcessor.apvts), filter(audioProcessor.apvts,"Filter Type","Filter Freq","Filter Res")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
    
    addAndMakeVisible(osc);
    //make adsr visible
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
}

Synth1AudioProcessorEditor::~Synth1AudioProcessorEditor()
{
}

//==============================================================================
void Synth1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    
    background = juce::ImageCache::getFromMemory(BinaryData::IMG_3911_png, BinaryData::IMG_3911_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    //g.fillAll(juce::Colours::black);
    
}

void Synth1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // set adsr bounds
    
    const auto paddingX=5;
    const auto paddingY1=35;
    const auto paddingY2=235;
    
    osc.setBounds(10,10,180,200);
    adsr.setBounds(getWidth()/2,0,getWidth()/2,getHeight());
    filter.setBounds(paddingX,200,300,200);
}


