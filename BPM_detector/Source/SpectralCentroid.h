/*
  ==============================================================================

    SpectralCentroid.h
    Created: 29 May 2019 12:00:51am
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include <queue>

/*
 The spectral centroid is a measure used in digital signal processing to characterise a spectrum.
 It indicates where the center of mass of the spectrum is located.
 Perceptually, it has a robust connection with the impression of brightness of a sound.
 */

//==============================================================================
/*
*/
class SpectralCentroid    : public juce::Component,
public juce::ChangeBroadcaster,
public juce::Thread
{
public:
    SpectralCentroid(PluginProcessor&);
    ~SpectralCentroid();

    void paint (juce::Graphics&) override;
    void resized() override;
    void run() override;
    void powerSpectrumFunction();
    
    void spectralCentroidFunction();

    void centersGrativtyFunction();
    
    void spectralCentroid();
    
    float centroid = 0;
    float sumOfSpectrum = 0;
    
    float centroidL = 0;
    float centroidR = 0;
    float centroidMid = 0;
    
    float averageCentroid=0;
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralCentroid)
    
    PluginProcessor& processor;
    float powerSpectrumL[PluginProcessor::fftSize];
    float powerSpectrumR[PluginProcessor::fftSize];
    
    float centersFreq[PluginProcessor::fftSize];

    
	int rangeFreq = round(processor.getSampleRate() / PluginProcessor::fftSize);
    
    float maxL=0;
    float maxR=0;
    
    float minL=0;
    float minR=0;
    
    std::queue<float> centroidQueue;
    std::queue<float> temporalCentroidQueue;
    
    
    int queueSize = 43;     //amout of centroid values to average to give the final result
    
    void fillCentroidQueue(float);
    
};
