/*
  ==============================================================================

    PanFeature.h
    Created: 28 May 2019 7:49:32pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class PanFeature    : public juce::Component,
public juce::ChangeBroadcaster,
public juce::Thread

{
public:
    PanFeature(PluginProcessor&);
    ~PanFeature();

    
    void paint (juce::Graphics&) override;
    void resized() override;
    void run() override;
    void panFeatureFunction();
    void calculateAveragePan();
    
    
    float panValue = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanFeature);
    
    PluginProcessor& processor;
    
    std::queue<float> panQueue;
    
    int queueSize = 43;     //amout of centroid values to average to give the final result

};
