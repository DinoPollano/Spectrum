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
    : AudioProcessorEditor (&p), processor (p), spectrumHeight (0),numSpecs(5)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

setResizeLimits (1024, 300, 2048, 600);
  	setSize (p.lastUIWidth, p.lastUIHeight);
	startTimer (100);
}

SpectrumAudioProcessorEditor::~SpectrumAudioProcessorEditor () {}
//==============================================================================
void SpectrumAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll (Colours::black);
  size_t specOrigin = spectrumBase;
  float colourIndex = 1.;
  for ( size_t t = 1; t <= numSpecs; t++)
  {
    std::vector<float> s = spectrumBuffer.getOne(t);
    std::transform (
        s.begin (), s.end (), s.begin (),
        std::bind1st (std::multiplies<float> (), (spectrumHeight*2 / t)));
    
    g.setColour (Colour::fromFloatRGBA(colourIndex, colourIndex, colourIndex, 1));
    colourIndex -= 0.1;
    float  lastVal   = 0;
    float  val       = 0;
    size_t x         = 0;
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
    if(specOrigin < 0)
    {
      specOrigin = 0;
    }
  }
}
void SpectrumAudioProcessorEditor::resized ()
{
	Rectangle<int> r (getLocalBounds ().reduced (8));

	numPoints = processor.getFFtSize () / 6;
	xCords.assign (numPoints, 0.);
	spectrumHeight = r.getHeight ();
	spacing        = std::round (r.getWidth () / numPoints);
	spectrumWidth  = r.getWidth ();
	spectrumBase   = r.getBottom ();
	originX        = r.getX ();
	endX           = r.getRight ();
	xCords[0]      = originX;
	std::iota (xCords.begin (), xCords.end (), 1);
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::multiplies<float> (), (spacing)));
	std::transform (xCords.begin (), xCords.end (), xCords.begin (),
	                std::bind1st (std::plus<float> (), (originX)));
  
  spectrumBuffer.init(numSpecs, std::vector<float, std::allocator<float> > (spectrumWidth,0.));
}

void SpectrumAudioProcessorEditor::timerCallback ()
{
	std::vector<float> s = processor.getSPectrum ();
  std::vector<float> fullSpec(spectrumWidth,0.);
  size_t specIndex = 1;
  float  val       = 0;
  size_t x         = 0;
  for (size_t i = originX; i < endX; i++)
  {
    if (xCords[specIndex] == i)
    {
      val = s[specIndex];
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
  spectrumBuffer.insertOne(fullSpec);
	repaint ();
}

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
