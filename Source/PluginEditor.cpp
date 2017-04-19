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
    : AudioProcessorEditor (&p), processor (p), spectrumHeight (0), numSpecs (4)
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
	size_t specOrigin  = spectrumBase;
	float  colourIndex = 1.;
  float colourAmount = 1. / numSpecs;
	for (size_t t = 1; t <= numSpecs; t++)
	{
		std::vector<float> s = spectrumBuffer.getOne (t);

		std::transform (
		    s.begin (), s.end (), s.begin (),
		    std::bind1st (std::multiplies<float> (), (spectrumHeight * 2 / t)));

		g.setColour (
		    Colour::fromFloatRGBA (colourIndex, colourIndex, colourIndex, 1));
		colourIndex -= colourAmount;
		float  lastVal = 0;
		float  val     = 0;
		size_t x       = 0;
		for (size_t i = originX; i < endX; i++)
		{
			val = s[x];
			if (std::abs (val - lastVal) > 1)
			{
				float start = (lastVal > val) ? val : lastVal;
				float end   = (lastVal < val) ? val : lastVal;
				for (float t = start; t < end; t++)
				{
					g.setPixel (i, std::round (specOrigin - t));
				}
			}
			else
			{
				g.setPixel (i, std::round (specOrigin - val));
			}
			lastVal = val;
			x++;
		}
		specOrigin -= 10;
	}
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));
  Rectangle<int> controlPanel(r.removeFromTop(20));
	Rectangle<int> spectrumSection (r);
  
	numPoints      = processor.getFFtSize ()/4;
	spectrumHeight = spectrumSection.getHeight ();
	spacing        = std::round (spectrumSection.getWidth () / numPoints);
	spectrumWidth  = spectrumSection.getWidth ();
	spectrumBase   = spectrumSection.getBottom ();
	originX        = spectrumSection.getX ();
	endX           = spectrumSection.getRight ();
	xCords.assign (numPoints, 0.);
	xCords[0] = originX;
	std::iota (xCords.begin (), xCords.end (), 1);
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::multiplies<float> (), (spacing)));
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::plus<float> (), (originX)));

	spectrumBuffer.init (
	    numSpecs, std::vector<float, std::allocator<float> > (spectrumWidth, 0.));
	processor.lastUIWidth  = getWidth ();
	processor.lastUIHeight = getHeight ();
}

void SpectrumAudioProcessorEditor::timerCallback ()
{
	std::vector<float> s = processor.getSPectrum ();
	std::vector<float> fullSpec (spectrumWidth, 0.);
	size_t             specIndex = 1 ;
	float              val       = 0.f;
	size_t             x         = 0;

	auto linInterp = [this](float x0, float x1, float x2, float y0,
	                        float y2) -> float {
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
	};

	for (size_t i = originX; i < endX; i++)
	{
		if (xCords[specIndex] == i)
		{
			specIndex++;
		}
		else
		{
			val = linInterp (static_cast<float> (xCords[specIndex - 1]),
			                 static_cast<float> (i),
			                 static_cast<float> (xCords[specIndex]),
			                 static_cast<float> (s[specIndex - 1]),
			                 static_cast<float> (s[specIndex]));
		}
		fullSpec[x] = val;
		x++;
	}

	spectrumBuffer.insertOne (fullSpec);
	repaint ();
}
