/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "synthSound.h"
#include "synthVoice.h"


//==============================================================================
Synth1AudioProcessor::Synth1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this,nullptr,"Parameters",createParams()) 
#endif
{
    synth.addSound(new synthSound());
    synth.addVoice(new synthVoice()); 
}

Synth1AudioProcessor::~Synth1AudioProcessor()
{
}

//==============================================================================
const juce::String Synth1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Synth1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Synth1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Synth1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Synth1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Synth1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Synth1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Synth1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Synth1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Synth1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Synth1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for(int i=0;i<synth.getNumVoices();i++){
        if(auto voice = dynamic_cast<synthVoice*>(synth.getVoice(i))){
            voice->prepareToPlay(sampleRate,samplesPerBlock,getTotalNumInputChannels());
        }
    }
    
    filter.prepareToPlay(sampleRate,samplesPerBlock,getTotalNumOutputChannels());
    
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Synth1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Synth1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Synth1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    for (int j = 0 ;j<synth.getNumVoices();++j){
        if(auto voice=dynamic_cast<synthVoice*>(synth.getVoice(j))){
            //OSC controls
            //ADSR (attack,decay,sustain,release)
            //LFO (Low Frequency oscillation)
            
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            
            auto& oscWaveChoice=*apvts.getRawParameterValue("OSC1WAVETYPE");
            auto& fmDepth=*apvts.getRawParameterValue("FMDEPTH");
            auto& fmFreq=*apvts.getRawParameterValue("FMFREQ");
            
            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator().setFmParams(fmDepth, fmFreq);
            voice->update(attack.load(), decay.load(), sustain.load(), release.load());
           
        }
    }


    
    synth.renderNextBlock(buffer,midiMessages,0,buffer.getNumSamples());
    
    // Filter stuff
    auto& filterType=*apvts.getRawParameterValue("Filter Type");
    auto& filterCutoff=*apvts.getRawParameterValue("Filter Freq");
    auto& filterRes=*apvts.getRawParameterValue("Filter Res");
    
    filter.updateParameters(filterType, filterCutoff, filterRes);
    
    filter.process(buffer);
}

//==============================================================================
bool Synth1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Synth1AudioProcessor::createEditor()
{
    return new Synth1AudioProcessorEditor (*this);
}

//==============================================================================
void Synth1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Synth1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Synth1AudioProcessor();
}
// Value Tree

juce::AudioProcessorValueTreeState::ParameterLayout Synth1AudioProcessor::createParams(){
    // combobox: switch oscillator
   
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    //OSC select
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"OSC",1}, "Oscillator", juce::StringArray{"Sine","Saw","Square"}, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"FMFREQ",1},"FM Frequency",juce::NormalisableRange<float>{0.0f,1000.0f,0.01f,0.3f},0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"FMDEPTH",1},"FM Depth",juce::NormalisableRange<float>{0.0f,1000.0f,0.01f,0.3f},0.0f)); // changes the volatility of oscillator 'gain'
    
    
    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ATTACK",1},"Attack",juce::NormalisableRange<float>{0.1f,1.0f,},0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DECAY",1},"Decay",juce::NormalisableRange<float>{0.1f,1.0f,},0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"SUSTAIN",1},"Sustain",juce::NormalisableRange<float>{0.1f,1.0f,},1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RELEASE",1},"Release",juce::NormalisableRange<float>{0.1f,3.0f,},0.4f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"OSC1WAVETYPE",1},"Osc 1 Wave Type", juce::StringArray{"Sine","Saw","Square"},0));
    
    
    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"Filter Type",1}, "Filter Type", juce::StringArray{"Low-Pass","Band-Pass","Square"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Filter Freq",1},"Filter Frequency",juce::NormalisableRange<float>{20.0f,20000.0f,0.1f,0.6f},200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Filter Res",1},"Filter Resonance",juce::NormalisableRange<float>{1.0f,10.0f,0.1f},1.0f));
    
    return {params.begin(),params.end()};
}
