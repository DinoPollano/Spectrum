/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DinosusefulStuff.h"
#include <vector>
#include <algorithm>
#include <memory>
using dino::FFTWrapper32;
using dino::FFTWrapper;
using dino::circularBuffer;
//==============================================================================
/**
*/
class SpectrumAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SpectrumAudioProcessor();
    ~SpectrumAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) ;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
  
    //
  std::vector<float> getSpectrum ();
  size_t getFFtSize(){return fftSize;};
  int lastUIWidth, lastUIHeight;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumAudioProcessor)
  circularBuffer<float> circuBuff;
  std::unique_ptr<float[]> m_XF;
  std::vector<float> m_xn;
  std::vector<float> spectrum;
  FFTWrapper32 fftCalc;
	size_t  fftSize;
  size_t blockCount;
  float* monoInputStream;
  float scaleAfterSumming;
  
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
