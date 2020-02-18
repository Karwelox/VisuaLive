/*
  ==============================================================================

    SpectralCentroid.h
    Created: 29 May 2019 12:00:51am
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include <queue>

//==============================================================================
/*
*/
class SpectralCentroid    : public Component,
                            public Thread,
                            public ChangeBroadcaster
{
public:
    SpectralCentroid(PluginProcessor&);
    ~SpectralCentroid();

    void paint (Graphics&) override;
    void resized() override;
    void run();
    void powerSpectrumFunction();
    
    void spectralCentroidFunction();

    void centersGrativtyFunction();
    
    //nuova funzione
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
    
    
    int queueSize = 20;
    
    void fillCentroidQueue(float);
    
};
