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
    : AudioProcessorEditor (&p),
      processor (p),
      spectrumHeight (0)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize (1024, 300);
  startTimer(100);

}

SpectrumAudioProcessorEditor::~SpectrumAudioProcessorEditor () {}
//==============================================================================
void SpectrumAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::white);
	
  std::vector<float> s = processor.getSPectrum();
  std::transform(s.begin(), s.end(), s.begin(), std::bind1st(std::multiplies<float>(), spectrumHeight));
  g.setColour (Colours::black);
  for( size_t i = 0; i < numPoints; i++)
  {
	g.drawLine(xCords[i], 0.,xCords[i], s[i]  ,1.f);
    
  }
  
}
void SpectrumAudioProcessorEditor::resized ()
{
  numPoints = processor.getFFtSize ()/2;
  xCords.assign (numPoints, 0.);
	auto bounds    = getLocalBounds ();
	spectrumHeight = bounds.getHeight () - 10;
	spectrumWidth  = bounds.getWidth () - 10;
	spacing        = spectrumWidth / numPoints;
	std::iota (xCords.begin (), xCords.end (), 1);
  std::transform(xCords.begin(), xCords.end(), xCords.begin(), std::bind1st(std::multiplies<float>(),spacing));
	originY = bounds.getHeight () - 10;
  
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}

void SpectrumAudioProcessorEditor::timerCallback()
{
  repaint();
}
