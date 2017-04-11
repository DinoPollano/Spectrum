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
	std::transform (
	    s.begin (), s.end (), s.begin (),
	    std::bind1st (std::multiplies<float> (), (spectrumHeight * 2)));
	g.setColour (Colours::whitesmoke);
	size_t specIndex = 1;
	float  lastVal   = 0;
	float  val       = 0;
	size_t x         = 0;
	for (size_t i = originX; i < endX; i++)
	{
		if (xCords[specIndex] == i)
		{
			val = s[specIndex];
			x   = xCords[specIndex];
			specIndex++;
		}
		else
		{
			val = linInterp (static_cast<float> (xCords[specIndex - 1]),
			                 static_cast<float> (i),
			                 static_cast<float> (xCords[specIndex]),
			                 static_cast<float> (s[specIndex - 1]),
			                 static_cast<float> (s[specIndex]));
			x = i;
		}
		g.setPixel (x, std::round (originY - val));
    if((val - lastVal) > 1)
    {
      for(float t = lastVal; t < val; t++)
      {
        g.setPixel (x, std::round (originY - t));
      }
    }
    if((lastVal - val) > 1)
    {
      for(float t = val; t < lastVal; t++)
      {
        g.setPixel (x, std::round (originY - t));
      }
    }
		lastVal = val;
	}
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));

	numPoints = processor.getFFtSize () / 4;
	xCords.assign (numPoints, 0.);
	spectrumHeight = r.getHeight ();
	spacing        = std::round (r.getWidth () / numPoints);
	spectrumWidth  = r.getWidth ();
	originY        = r.getBottom ();
	originX        = r.getX ();
	endX           = r.getRight ();
	xCords[0]      = originX;
	std::iota (xCords.begin (), xCords.end (), 1);
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::multiplies<float> (), (spacing)));
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::plus<float> (), (originX)));
}
void  SpectrumAudioProcessorEditor::timerCallback () { repaint (); }
float SpectrumAudioProcessorEditor::linInterp (float x0, float x1, float x2,
                                               float y0, float y2)
{
	float y1 = 0.;
	if (y0 == 0. && y2 == 0)
	{
		y1 = 0.;

		return y1;
	}
	y1 = y0 + ((x1 - x0) * (y2 - y0)) / (x2 - x0);

	if (std::abs (y1) >= spectrumHeight)
	{
		y1 = spectrumHeight - 1;
	}

	return y1;
}
