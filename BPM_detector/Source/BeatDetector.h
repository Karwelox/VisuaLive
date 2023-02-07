/*
  ==============================================================================

    BeatDetector.h
    Created: 28 May 2019 10:57:10pm
    Author:  Carmelo Fascella

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <iostream>
#include <queue>
#include <stack>

//==============================================================================
/*
*/
class BeatDetector    : public juce::Component,
public juce::ChangeBroadcaster,
public juce::Thread


{
public:
    BeatDetector(PluginProcessor&);
    ~BeatDetector();

    void paint (juce::Graphics&) override;
    void resized() override;
    void run() override;
    //void runAlgorithm();

    
    //=============================
    //BPM DETECTION METHODS
    void beatDetection();
    float averageQueue(std::queue<std::vector<float>> temporalQueue, int index);
    float calculateFFTEnergyInRange(int indexRange);
    void thresholdCalculus();
    float varianceEnergyHistory(float average, std::queue<std::vector<float>> tempQueue, int index);
    //=============================
    
    int beatTime = 0; 

	bool beforeTransient = false;
	bool transient = false;
	bool reducedTransient = false; //per la velocity
	double transientStartTime = 0;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeatDetector)
    
    PluginProcessor& processor;
    
    int freqRes = round(processor.getSampleRate() / PluginProcessor::fftSize);      //dim = frequency resolution: how many frequencies information is
                                                                                    //contained into one single fft sample. (remember we have fftSize
                                                                                    //samples in one fft block)
    
    int numFFTBlocksOneSecond = round(processor.getSampleRate() / PluginProcessor::fftSize); //num of fft buffers in one second of input processing.
                                                                                             //In this case is the same of freq res
    int oldCount=0;
    int energyIndex = 0;
    
    std::queue<std::vector<float>> energyHistory;   //queue of vectors. each vector has two values: [kick range fft energy, snare range fft energy]
    
    std::priority_queue<float> bpmQueue;
    
    
    float BPMthreshold[2];
    
    double timeAverage = 0; //manual mode

    const int lowFreqKickRange = 60;
    const int highFreqKickRange = 130;
    const int lowFreqSnareRange = 301;
    const int highFreqSnareRange = 750;
    
    
    const int lowIndexKickRange = round(lowFreqKickRange / freqRes);      //sample index = frequency / amount of frequency samples in one fft.
    const int highIndexKickRange = round(highFreqKickRange / freqRes);
    const int lowIndexSnareRange = round(lowFreqSnareRange / freqRes);
    const int highIndexSnareRange = round(highFreqSnareRange / freqRes);
    
    const int bandKick = (highIndexKickRange - lowIndexKickRange) * 2;
    const int bandSnare =( highIndexSnareRange-lowIndexSnareRange) * 2;
    
};
