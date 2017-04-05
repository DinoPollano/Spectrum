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
    : AudioProcessorEditor (&p), processor (p), spectrumHeight (0)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize (1024, 300);
	startTimer (100);
}

SpectrumAudioProcessorEditor::~SpectrumAudioProcessorEditor () {}
//==============================================================================
void SpectrumAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::black);

	std::vector<float> s = processor.getSPectrum ();
	std::transform (s.begin (), s.end (), s.begin (),
	                std::bind1st (std::multiplies<float> (), spectrumHeight));
	g.setColour (Colours::grey);
	for (size_t i = 0; i < numPoints; i++)
	{
//		g.drawLine (xCords[i], originY, xCords[i], originY - s[i], 1.f);
    g.setPixel(xCords[i],  originY - s[i]);
	}
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));

	numPoints = processor.getFFtSize () / 4;
	xCords.assign (numPoints, 0.);
	spectrumHeight = r.getHeight ();
	spectrumWidth  = r.getWidth ();
	spacing        = spectrumWidth / numPoints;
	std::iota (xCords.begin (), xCords.end (), 1);
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::multiplies<float> (), spacing));
	originY = r.getHeight ();

}

void SpectrumAudioProcessorEditor::timerCallback () { repaint (); }
