/*
  ==============================================================================

    FilterComponent.cpp
    Created: 30 May 2024 7:28:09pm
    Author:  George Anders

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId, juce::String filterFreqId, juce::String filterResId)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
 
    juce::StringArray choices{"Low-Pass","Band-Pass","High-Pass"};
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);
     
    filterTypeSelectorAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts,filterTypeSelectorId, filterTypeSelector);
    
 
    
    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);
    
    setSliderWithLabel(filterFreqSlider, filterFreqLabel , apvts, filterFreqId, filterFreqAttachment);
    setSliderWithLabel(filterResSlider, filterResLabel , apvts, filterResId, filterResAttachment);
    
    
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    
}

void FilterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    /*
    const auto startY=15;
    const auto sliderPosY=60;
    const auto sliderWidth=100;
    const auto sliderHeight=50;
    const auto labelYOffset=10;
    const auto labelHeight=10;
    
    
    filterTypeSelector.setBounds(10,startY+5,90,30);
    filterSelectorLabel.setBounds(filterTypeSelector.getX(),filterTypeSelector.getY()-labelYOffset,
                                  filterFreqSlider.getWidth(),labelHeight);

    //filterSelectorLabel.setBounds(5, startY,90, labelHeight);
    
    filterFreqSlider.setBounds(filterTypeSelector.getRight(),35,sliderWidth,sliderHeight);
        
    
    filterResLabel.setBounds(filterTypeSelector.getRight(), 20 ,filterResSlider.getWidth(),labelHeight);
    filterResSlider.setBounds(filterResLabel.getX(),filterResLabel.getY() - labelYOffset,sliderWidth,sliderHeight);*/
    
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth=bounds.getWidth() / 3 - padding;
    const auto sliderHeight=(bounds.getHeight())/2-10;
    const auto sliderStartX=0;
    const auto sliderStartY=0;
    
    filterTypeSelector.setBounds(sliderStartX,sliderStartY,sliderWidth-10, sliderHeight-40);
    filterFreqSlider.setBounds(filterTypeSelector.getRight()+3*padding,sliderStartY,sliderWidth, sliderHeight);
    filterResSlider.setBounds(filterFreqSlider.getRight()+padding,sliderStartY,sliderWidth, sliderHeight);
    filterSelectorLabel.setBounds(filterTypeSelector.getX(),filterTypeSelector.getY()+12,sliderWidth, sliderHeight);
    filterFreqLabel.setBounds(filterFreqSlider.getX()+padding,filterFreqSlider.getY()+55,sliderWidth-10, sliderHeight-10);
    filterResLabel.setBounds(filterResSlider.getX()+padding,filterResSlider.getY()+55,sliderWidth-10, sliderHeight-10);

}


void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment){
    
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
    
    attachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (apvts,paramId, slider);
    
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
