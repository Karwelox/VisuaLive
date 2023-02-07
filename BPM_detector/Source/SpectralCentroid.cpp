/*
  ==============================================================================

    SpectralCentroid.cpp
    Created: 29 May 2019 12:00:51am
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpectralCentroid.h"
#include "PluginProcessor.h"

//==============================================================================
SpectralCentroid::SpectralCentroid(PluginProcessor& p)
:  processor(p), juce::Thread("SpectralCentroidThread")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	centersGrativtyFunction();
 
}

SpectralCentroid::~SpectralCentroid()
{
}

void SpectralCentroid::paint (juce::Graphics& g)
{
   
}

void SpectralCentroid::resized()
{

}

void SpectralCentroid::centersGrativtyFunction()
{
    centersFreq[0] = rangeFreq;

    for(int i = 1; i < PluginProcessor::fftSize; i++)
    {
        centersFreq[i] = rangeFreq * (i + 1);
    }
}

void SpectralCentroid::run()
{
    spectralCentroid();
}


void SpectralCentroid::spectralCentroid(){
    
    centroid = 0;
    sumOfSpectrum = 0;
    
    //calcolo contenuto di ciascuna banda e la somma dello spettro
    for(int i = 0; i <= PluginProcessor::fftSize / 2; i++){
        //processor.fftData[i] = ( processor.fftDataL[i] + processor.fftDataR[i] + 0.0000001) / 2;  //offset per evitare divisione per 0
        sumOfSpectrum += processor.newFftData[i];
        
        centroid += processor.freqs[i] * processor.newFftData[i];       //frequency corresponding to the fft sample * magnitude of the fft sample.
    }
    
    centroid = (centroid+0.001)/(sumOfSpectrum+0.001);   //offset per evitare nan o inf
    

    
    if(centroidQueue.size()<queueSize){  //solo per le prime 10 iterazioni
        centroidQueue.push(centroid);
    }
    
    else{    //altrimenti elimino elemento più vecchio, aggiungo il più nuovo e calcolo la media
        centroidQueue.pop();
        centroidQueue.push(centroid);
        fillCentroidQueue(centroid);
    }
}



void SpectralCentroid::fillCentroidQueue(float centroid){
    
    float val=0, sum=0;
    
    temporalCentroidQueue = centroidQueue;

    while(temporalCentroidQueue.size() > 0)
    {
        val = temporalCentroidQueue.front();
        sum = sum + val;
        temporalCentroidQueue.pop();
    }

    averageCentroid = sum / queueSize;
}

