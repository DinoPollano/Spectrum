/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectrumAudioProcessor::SpectrumAudioProcessor ()
    : lastUIWidth (1024), lastUIHeight (600), fftSize (1024), blockCount (0)
{
	fftCalc.prepFFT (fftSize);
}

SpectrumAudioProcessor::~SpectrumAudioProcessor () {}
//==============================================================================
const String SpectrumAudioProcessor::getName () const
{
	return JucePlugin_Name;
}

bool SpectrumAudioProcessor::acceptsMidi () const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SpectrumAudioProcessor::producesMidi () const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

double SpectrumAudioProcessor::getTailLengthSeconds () const { return 0.0; }
int    SpectrumAudioProcessor::getNumPrograms ()
{
	return 1;  // NB: some hosts don't cope very well if you tell them there are 0
	           // programs,
	// so this should be at least 1, even if you're not really implementing
	// programs.
}

int  SpectrumAudioProcessor::getCurrentProgram () { return 0; }
void SpectrumAudioProcessor::setCurrentProgram (int index) {}
const String SpectrumAudioProcessor::getProgramName (int index)
{
	return String ();
}

void SpectrumAudioProcessor::changeProgramName (int           index,
                                                const String& newName)
{
}

//==============================================================================
void SpectrumAudioProcessor::prepareToPlay (double sampleRate,
                                            int    samplesPerBlock)
{
	circuBuff.init (fftSize, 0.);
	m_XF = std::make_unique<float[]> (fftSize / 2);
	spectrum.resize (fftSize / 2, 0.);
  monoInputStream = new float[samplesPerBlock];
  std::fill(monoInputStream, monoInputStream + samplesPerBlock, 0.f);
  scaleAfterSumming = 1.f/samplesPerBlock;
}

void SpectrumAudioProcessor::releaseResources ()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
	circuBuff.flush ();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectrumAudioProcessor::setPreferredBusArrangement (
    bool isInput, int bus, const AudioChannelSet& preferredSet)
{
	// Reject any bus arrangements that are not compatible with your plugin

	const int numChannels = preferredSet.size ();

#if JucePlugin_IsMidiEffect
	if (numChannels != 0)
		return false;
#elif JucePlugin_IsSynth
	if (isInput || (numChannels != 1 && numChannels != 2))
		return false;
#else
	if (numChannels != 1 && numChannels != 2)
		return false;

	if (!setPreferredBusArrangement (!isInput, bus, preferredSet))
		return false;
#endif

	return setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void SpectrumAudioProcessor::processBlock (AudioSampleBuffer& buffer,
                                           MidiBuffer&        midiMessages)
{
	const int totalNumInputChannels  = getTotalNumInputChannels ();
	const int totalNumOutputChannels = getTotalNumOutputChannels ();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear (i, 0, buffer.getNumSamples ());
  
  const float* block;
  for (size_t ch; ch < totalNumInputChannels; ch++) {
    	 block = buffer.getReadPointer (ch);
    std::transform(block, block+buffer.getNumSamples(), monoInputStream, monoInputStream, std::plus<float>());
  }
  std::transform(monoInputStream, monoInputStream+buffer.getNumSamples(), monoInputStream, std::bind1st(std::multiplies<float>(), scaleAfterSumming));

	circuBuff.insertMany (block, buffer.getNumSamples ());
	blockCount += buffer.getNumSamples ();
	if (blockCount >= (fftSize / 2) - 1)
	{
 		m_xn = circuBuff.getUnwrapped ();
		fftCalc.performHanningWindow (m_xn.data (), fftSize);
		fftCalc.calculateMagnitude (m_xn.data (), m_XF.get ());
		std::copy (m_XF.get (), m_XF.get () + (fftSize / 2), spectrum.begin ());
		blockCount = 0;
	}

	std::transform(monoInputStream, monoInputStream+buffer.getNumSamples(), monoInputStream, std::bind1st(std::multiplies<float>(), 0.f));
}

std::vector<float> SpectrumAudioProcessor::getSPectrum ()
{
	std::transform (spectrum.begin (), spectrum.end (), spectrum.begin (),
	                std::bind1st (std::multiplies<float> (), (2)));
	return spectrum;
};

//==============================================================================
bool SpectrumAudioProcessor::hasEditor () const
{
	return true;  // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpectrumAudioProcessor::createEditor ()
{
	return new SpectrumAudioProcessorEditor (*this);
}

//==============================================================================
void SpectrumAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void SpectrumAudioProcessor::setStateInformation (const void* data,
                                                  int         sizeInBytes)
{
	// You should use this method to restore your parameters from this memory
	// block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter ()
{
	return new SpectrumAudioProcessor ();
}
