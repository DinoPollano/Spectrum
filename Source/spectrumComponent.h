
/*
  ==============================================================================

    spectrumComponent.h
    Created: 25 Apr 2017 8:22:32pm
    Author:  Christofero Pollano

  ==============================================================================
*/

#ifndef spectrumComponent_H_INCLUDED
#define spectrumComponent_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class spectrumComponent : public Component
{
 public:
	spectrumComponent ()
	    : spectrumHeight (0),
	      maxNumSpecs (100),
	      numSpecs (maxNumSpecs),
	      specIndex (numSpecs),
	      spectrumLineStyle (0.8)
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
	}

	~spectrumComponent () {}
	void paint (Graphics& g) override
	{
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

	void resized () override
	{
		Rectangle<int> spectrumSection (getLocalBounds ());
		numPoints       = fftsize / 4;
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

		spectrumBuffer.init (
		    maxNumSpecs, std::vector<float, std::allocator<float>> (fftsize / 2, 0.));

		spectrumLineStyle.setJointStyle (juce::PathStrokeType::JointStyle::beveled);
	}
	void feedSpectrum (std::vector<float> s) { spectrumBuffer.insertOne (s); }
	void setFFTSize (size_t N)
	{
		jassert (N > 0);
		fftsize = N;
	}
	void setNumberOfSpectrums(size_t N)
  {
    jassert(N <= maxNumSpecs && N > 0);
    numSpecs = N;
    
  }
 private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spectrumComponent)
	float                              spectrumHeight;
	size_t                             spectrumWidth;
	float                              spacing;
	float                              numPoints;
	std::vector<size_t>                xCords;
	size_t                             originX         = 0;
	size_t                             spectrumBase    = 0;
	size_t                             endX            = 0;
	size_t                             spectrumSpacing = 0;
	std::size_t                maxNumSpecs;
  size_t numSpecs;
	size_t                             specIndex;
	circularBuffer<std::vector<float>> spectrumBuffer;
	PathStrokeType                     spectrumLineStyle;
	size_t                             fftsize;
};

#endif  // spectrumComponent_H_INCLUDED

