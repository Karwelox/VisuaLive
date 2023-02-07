/*
  ==============================================================================

    PanFeature.cpp
    Created: 28 May 2019 7:49:32pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PanFeature.h"
#include "PluginProcessor.h"

//==============================================================================
PanFeature::PanFeature (PluginProcessor& p):
processor(p), juce::Thread("PanThread")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PanFeature::~PanFeature()
{
}

void PanFeature::paint (juce::Graphics& g)
{
    
}

void PanFeature::resized()
{
    
}



void PanFeature::run()
{
    panFeatureFunction();
}

void PanFeature::panFeatureFunction()
{

    int panLeft = 0, panRight = 0;
    
    for (int i = 0; i < processor.fftSize / 2; i++)
    {
        if (processor.fftDataL[i] - processor.fftDataR[i] > 0)
        {
            panLeft++;
        }
        else if (processor.fftDataL[i] - processor.fftDataR[i] < 0)
        {
            panRight++;
        }
     
    }
    float currPanValue = (float)(panRight - panLeft)/ (PluginProcessor::fftSize / 2);
    
    //calculate average Panning
    if(panQueue.size()<queueSize){
        panQueue.push(currPanValue);
    }
    
    else{
        panQueue.pop();
        panQueue.push(currPanValue);
        calculateAveragePan();
    }
    
}

void PanFeature::calculateAveragePan(){
    
    float val=0, sum=0;
    
    std::queue<float> tempQueue = panQueue;

    while(tempQueue.size() > 0)
    {
        val = tempQueue.front();
        sum = sum + val;
        tempQueue.pop();
    }

    panValue = sum / queueSize;
}
