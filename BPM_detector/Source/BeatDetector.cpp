/*
  ==============================================================================

    BeatDetector.cpp
    Created: 28 May 2019 10:57:10pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "BeatDetector.h"
#include "PluginProcessor.h"

//==============================================================================
BeatDetector::BeatDetector (PluginProcessor& p): processor(p), juce::Thread("BeatDetectorThread")
{
    
}

BeatDetector::~BeatDetector()
{
}

void BeatDetector::paint (juce::Graphics& g)
{
    
}

void BeatDetector::resized()
{
    

}

void BeatDetector::run()
{
    beatDetection();
}


void BeatDetector::beatDetection() {

    float energyRange[2];
    
    energyRange[0] = calculateFFTEnergyInRange(0);   //calcolo energia del low range del singolo buffer
    energyRange[1] = calculateFFTEnergyInRange(1);   //calcolo energia del mid range del singolo buffer
    
    if (energyRange[0] == 0 && energyRange[1] == 0 || isnan(energyRange[0]) && isnan(energyRange[1])  ) {
        beforeTransient = true;   //vuoto prima dell'inizio dell'attacco
        //transientAttack.setText("transientAttack: off");
    }
    else if (beforeTransient) {
        beforeTransient = false;
        //transientAttack.setText("transientAttack: on");
        transientStartTime = juce::Time::getMillisecondCounterHiRes() * 0.001;
        transient = true;  //inizio dell'attacco
		reducedTransient = true; //attacco ridotto per la velocity
    }
    
    
    std::vector<float> fftResult;
    fftResult.reserve(2);
    for(int i = 0; i < 2; i++)
    {
        fftResult.push_back(energyRange[i]);
    }
    
    energyHistory.push(fftResult);
    
    
    if (energyIndex >= numFFTBlocksOneSecond-1) {
        
        thresholdCalculus();  //calcolus BPMThreshold
        
        //Case: Beat is detected!
        if (energyRange[0] - 0.05 > BPMthreshold[0] || energyRange[1] - 0.005 > BPMthreshold[1]) {
            //confronto con 1 secondo di delay
            //setNoteNumber((int)(Time::getMillisecondCounterHiRes()) % 128);
            //==============
            beatTime = (int)(juce::Time::getMillisecondCounterHiRes()) % 128;

            sendChangeMessage();        //trigger changeListenerCallback() in the editor
        }
        energyHistory.pop();        //remove oldest value in the queue
    }
    
    else {
        energyIndex++;
    }
    
    
    
}

//fa la media dell energy history buffer
float BeatDetector::averageQueue(std::queue<std::vector<float>> temporalQueue, int index)
{
    std::vector<float> val;
    float sum = 0;
    
    if(index == 0)
    {
        while(temporalQueue.size() > 0)
        {
            val = temporalQueue.front();
            sum = sum + val[0];
            temporalQueue.pop();
        }
    }
    
    else if(index == 1)
    {
        while(temporalQueue.size() > 0)
        {
            val = temporalQueue.front();
            sum = sum + val[1];
            temporalQueue.pop();
        }
    }
    
    
    return sum / numFFTBlocksOneSecond;
    
    
}


float BeatDetector::calculateFFTEnergyInRange(int indexRange){
    
    float sum = 0;
    
    switch(indexRange)
    {
        case 0:     //only kick range
            for (int i = lowIndexKickRange; i <= highIndexKickRange; i++) {
                sum = sum + processor.fftDataL[i] + processor.fftDataR[i];
            }
            sum = sum / bandKick;
            break;
        
        case 1:     //only snare range
            for (int i = lowIndexSnareRange; i <= highIndexSnareRange; i++) {
                sum = sum + processor.fftDataL[i] + processor.fftDataR[i];
            }
            sum = sum / bandSnare;
            break;
        
        case 2:     //kick + snare range
            for (int i = lowIndexKickRange; i <= highIndexKickRange; i++) {
                sum = sum + processor.fftDataL[i] + processor.fftDataR[i];
            }
            for (int i = lowIndexSnareRange; i <= highIndexSnareRange; i++) {
                sum = sum + processor.fftDataL[i] + processor.fftDataR[i];
            }
    }
    return sum;
}



void BeatDetector::thresholdCalculus() {
    
    std::queue<std::vector<float>> temporalQueue = energyHistory;
    
    float average[2];
    average[0] = averageQueue(temporalQueue, 0);
    average[1] = averageQueue(temporalQueue, 1);
    
    temporalQueue = energyHistory;
    //float average = averageEnergyHistory();
    float variance[2];
    variance[0] = varianceEnergyHistory(average[0], temporalQueue, 0);
    variance[1] = varianceEnergyHistory(average[1], temporalQueue, 1);
    //return (-0.0025714 * variance)*average + 1.5142857;
    
    BPMthreshold[0] = (-15 * variance[0] + 1.55) * average[0];
    BPMthreshold[1] = (-15 * variance[1] + 1.55) * average[1];
}


float BeatDetector::varianceEnergyHistory(float average, std::queue<std::vector<float>> tempQueue, int index) {
    
    std::vector<float> val;
    float sum = 0;
    
    if(index == 0)
    {
        while(tempQueue.size() > 0)
        {
            val=tempQueue.front();
            sum = sum + (val[0] - average)*(val[0] - average);
            tempQueue.pop();
        }
    }
    
    else if(index == 1)
    {
        while(tempQueue.size() > 0)
        {
            val=tempQueue.front();
            sum = sum + (val[1] - average)*(val[1] - average);
            tempQueue.pop();
        }
    }
    
    return sum / numFFTBlocksOneSecond;
    
}
