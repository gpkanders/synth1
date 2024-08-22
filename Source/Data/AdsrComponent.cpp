/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 19 Mar 2024 6:13:49pm
    Author:  George Anders

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    using SliderAttachment=juce::AudioProcessorValueTreeState::SliderAttachment;
    
    attackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    attackLabel.setFont(15.0f);
    attackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(attackLabel);
    
    decayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    decayLabel.setFont(15.0f);
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(decayLabel);
    
    sustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    sustainLabel.setFont(15.0f);
    sustainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sustainLabel);
    
    releaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    releaseLabel.setFont(15.0f);
    releaseLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(releaseLabel);
    
    attackAttachment=std::make_unique<SliderAttachment>(apvts,"ATTACK",attackSlider);
    decayAttachment=std::make_unique<SliderAttachment>(apvts,"DECAY",decaySlider);
    sustainAttachment=std::make_unique<SliderAttachment>(apvts,"SUSTAIN",sustainSlider);
    releaseAttachment=std::make_unique<SliderAttachment>(apvts,"RELEASE",releaseSlider);
    
    
    //style
    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
    
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("AdsrComponent", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
    
    //g.fillAll(juce::Colours::black);
}

void AdsrComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth=bounds.getWidth() / 4-(padding/2);
    const auto sliderHeight=(bounds.getHeight())/2;
    const auto sliderStartX=0;
    const auto sliderStartY=0;
    
    attackSlider.setBounds(sliderStartX,sliderStartY,sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight()+padding,sliderStartY,sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight()+padding,sliderStartY,sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight()+padding,sliderStartY,sliderWidth, sliderHeight);
    
    attackLabel.setBounds(sliderStartX,sliderStartY+85,sliderWidth, sliderHeight);
    decayLabel.setBounds(attackSlider.getRight()+padding,sliderStartY+85,sliderWidth, sliderHeight);
    sustainLabel.setBounds(decaySlider.getRight()+padding,sliderStartY+85,sliderWidth, sliderHeight);
    releaseLabel.setBounds(sustainSlider.getRight()+padding,sliderStartY+85,sliderWidth, sliderHeight);

}
void AdsrComponent::setSliderParams(juce::Slider& slider){
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,50,35);
    addAndMakeVisible(slider);
}
