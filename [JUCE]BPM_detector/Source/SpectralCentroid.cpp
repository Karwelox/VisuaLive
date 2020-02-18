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
:Thread("SpectralCentroidThread"), processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	centersGrativtyFunction();
 
}

SpectralCentroid::~SpectralCentroid()
{
}

void SpectralCentroid::paint (Graphics& g)
{
   
}

void SpectralCentroid::resized()
{

}

void SpectralCentroid::run()
{
    //spectralCentroidFunction();
    
    spectralCentroid();
}

void SpectralCentroid::spectralCentroid(){
    
    centroid = 0;
    sumOfSpectrum = 0;
    
    
    
    
    //calcolo contenuto di ciascuna banda e la somma dello spettro
    for(int i = 0; i <= PluginProcessor::fftSize / 2; i++){
        //processor.fftData[i] = ( processor.fftDataL[i] + processor.fftDataR[i] + 0.0000001) / 2;  //offset per evitare divisione per 0
        sumOfSpectrum += processor.newFftData[i];
        
        centroid += processor.freqs[i] * processor.newFftData[i];
    }
    
     //only to see the data of fft
    /*for(int i = 0; i <= PluginProcessor::fftSize / 2 ; i++){
        
        printf("%f ", processor.newFftData[i]);
    }
    printf("\n\n");*/
    
    
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
    
    
    //printf("\n");
    while(temporalCentroidQueue.size() > 0)
    {
        val = temporalCentroidQueue.front();
        //printf("%f  ", val);
        sum = sum + val;
        temporalCentroidQueue.pop();
    }
    //printf("\n");
        
    averageCentroid = sum / queueSize;
    //printf("\n %f", averageCentroid);
    
    
    
    
}

/*void SpectralCentroid::spectralCentroid(){
    
    centroid = 0;
    sumOfSpectrum = 0;
    
    //calcolo contenuto di ciascuna banda e la somma dello spettro
    for(int i = 0; i < PluginProcessor::fftSize; i++){
        processor.fftData[i] = ( processor.fftDataL[i] + processor.fftDataR[i] + 0.01) / 2;  //offset per evitare divisione per 0
        sumOfSpectrum += processor.fftData[i];
    }
    
    
    for(int i = 0; i < PluginProcessor::fftSize; i++){
        centroid += processor.freqs[i] * processor.fftData[i];
    }
    
    
    centroid = centroid/sumOfSpectrum;
    
}*/


//////          VECCHIE FUNZIONI             //////


/*void SpectralCentroid::spectralCentroidFunction()
{
    float numL = 0;
    float numR = 0;
    float denL = 0;
    float denR = 0;
    
    powerSpectrumFunction();
    
    
    for(int i = 0; i <= PluginProcessor::fftSize / 2 - 1; i++)
    {
        numL +=  log2(centersFreq[i] / 1000 ) * powerSpectrumL[i + 1];
        numR +=  log2(centersFreq[i] / 1000 ) * powerSpectrumR[i + 1];
        
        denL += powerSpectrumL[i + 1];
        denR += powerSpectrumR[i + 1];
    }
    
    if (denL < pow(10, -25))
        centroidL = 0;
    else
        centroidL = numL / denL;
    
    if (denR < pow(10, -25))
        centroidR = 0;
    else
        centroidR = numR / denR;
    
    centroidMid = (centroidL + centroidR) / 2;
    
    //printf("\ndiff: %.5f", numL - numR);
    
    
    
    //printf("\nleft:%.3f   mid:%.3f   right: %.3f", centroidL, centroidMid, centroidR);
    //sendChangeMessage();
    
    
}

//calculation of P(k)
void SpectralCentroid::powerSpectrumFunction()
{
    //printf("\n");
    for(int i = 0; i < PluginProcessor::fftSize; i++)
    {
        powerSpectrumL[i] = processor.fftDataL[i] * processor.fftDataL[i];
        powerSpectrumR[i] = processor.fftDataR[i] * processor.fftDataR[i];
        
        //printf("%.4f ", processor.fftDataL[i]);

    }
    
    //modify the spectrum, it starts from 1, the value i = 0 is discarded
    powerSpectrumL[1] = powerSpectrumL[0] + powerSpectrumL[1];
    powerSpectrumR[1] = powerSpectrumR[0] + powerSpectrumR[1];
    //printf("\n");
    
}
*/

//NON SI PUO DECOMMENTARE... (è chiamata nel costruttore...)
void SpectralCentroid::centersGrativtyFunction()
{
	centersFreq[0] = rangeFreq;

    for(int i = 1; i < PluginProcessor::fftSize; i++)
    {
        centersFreq[i] = rangeFreq * (i + 1);
    }
}
