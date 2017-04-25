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
      spectrumHeight (0),
      numSpecs (64),
      specIndex (numSpecs),
      spectrumLineStyle (1.f)

{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	setResizable (true, true);
	setSize (p.lastUIWidth, p.lastUIHeight);
	startTimer (100);
}

SpectrumAudioProcessorEditor::~SpectrumAudioProcessorEditor () {}
//==============================================================================
void SpectrumAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::black);
	g.setColour (Colours::whitesmoke);
	g.drawFittedText ("Spectrum", title, Justification::Flags::topLeft, 1);
	size_t specOrigin = spectrumBase - (spectrumSpacing * numSpecs);

	float colourAmount = 1.f / numSpecs;
	float colourIndex  = 2.f * colourAmount;
	for (size_t t = specIndex; t >= 1; --t)
	{
		std::vector<float> s = spectrumBuffer.getOne (t);
		std::transform (
		    s.begin (), s.end (), s.begin (),
		    std::bind1st (std::multiplies<float> (), (spectrumHeight * 2 / t)));
		g.setColour (
		    Colour::fromFloatRGBA (colourIndex, colourIndex, colourIndex, 1));
		colourIndex += colourAmount;

		Path spectrumLine;
		spectrumLine.startNewSubPath (xCords[0], specOrigin);
		for (size_t i = 1; i < static_cast<size_t> (numPoints); i++)
		{
			spectrumLine.lineTo (xCords[i], specOrigin - s[i]);
		};
		specOrigin += spectrumSpacing;
		g.strokePath (spectrumLine, spectrumLineStyle);

		specIndex = t;
		if (specIndex <= 1)
		{
			specIndex = numSpecs;
		}
	}
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));
	Rectangle<int> controlPanel (r.removeFromTop (50));
	title = controlPanel.removeFromLeft (100);

	Rectangle<int> spectrumSection (r);

	numPoints       = processor.getFFtSize () / 4;
	spectrumHeight  = spectrumSection.getHeight ();
	spectrumSpacing = std::round (spectrumHeight / numSpecs);
	spacing         = spectrumSection.getWidth () / numPoints;
	spectrumBase    = spectrumSection.getBottom ();
	originX         = spectrumSection.getX ();
	endX            = spectrumSection.getRight ();
	xCords.assign (numPoints, 0.);
	xCords[0] = originX;
	std::iota (xCords.begin (), xCords.end (), 1);
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::multiplies<float> (), (spacing)));
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::plus<float> (), (originX)));

	spectrumBuffer.init (numSpecs, std::vector<float, std::allocator<float> > (
	                                   processor.getFFtSize () / 2, 0.));
	processor.lastUIWidth  = getWidth ();
	processor.lastUIHeight = getHeight ();
	spectrumLineStyle.setJointStyle (juce::PathStrokeType::JointStyle::beveled);
}

void SpectrumAudioProcessorEditor::timerCallback ()
{
	std::vector<float> s = processor.getSPectrum ();
	spectrumBuffer.insertOne (s);
	repaint ();
}
