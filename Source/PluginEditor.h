/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <numeric>
#include <algorithm>

//==============================================================================
/**
*/
class SpectrumAudioProcessorEditor : public AudioProcessorEditor, private Timer
{
 public:
	SpectrumAudioProcessorEditor (SpectrumAudioProcessor&);
	~SpectrumAudioProcessorEditor ();

	//==============================================================================
	void paint (Graphics&) override;
	void resized () override;
	void timerCallback () override;

 private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SpectrumAudioProcessor& processor;
	size_t                  spectrumHeight;
	size_t                  spectrumWidth;
	size_t                  spacing;
	size_t                  numPoints;
	std::vector<size_t>     xCords;
	size_t                  originX = 0;
	size_t                  originY = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumAudioProcessorEditor)
};

#endif  // PLUGINEDITOR_H_INCLUDED
