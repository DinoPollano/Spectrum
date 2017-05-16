/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectrumAudioProcessorEditor::SpectrumAudioProcessorEditor (
    SpectrumAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)

{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setResizable (true, true);
	setSize (p.lastUIWidth, p.lastUIHeight);
	addAndMakeVisible (specWindow);
	startTimer (100);
}

SpectrumAudioProcessorEditor::~SpectrumAudioProcessorEditor () {}
//==============================================================================
void SpectrumAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::black);
	g.setColour (Colours::whitesmoke);
	g.drawFittedText ("Spectrum", title, Justification::Flags::topLeft, 1);
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));
	Rectangle<int> controlPanel (r.removeFromTop (50));
	title = controlPanel.removeFromLeft (100);
  specWindow.setNumberOfSpectrums(40);
  specWindow.setFFTSize(processor.getFFtSize());
  specWindow.setBounds(r);
	processor.lastUIWidth  = getWidth ();
	processor.lastUIHeight = getHeight ();
}

void SpectrumAudioProcessorEditor::timerCallback ()
{
  specWindow.feedSpectrum(processor.getSpectrum());
	repaint ();
}
