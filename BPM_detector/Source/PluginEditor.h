/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PanFeature.h"
#include "BeatDetector.h"
#include "SpectralCentroid.h"
#include <iostream>
#include <queue>
#include <stack>



//==============================================================================
/**
*/
class PluginEditor : public juce::AudioProcessorEditor,
public juce::ChangeListener,
public juce::Slider::Listener,
private juce::Timer,
public juce::OSCSender
    
{
public:
	PluginEditor(PluginProcessor&);
	~PluginEditor();

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void sliderValueChanged(juce::Slider* slider) override;

	void setNoteNumber(int faderNumber, int velocity);
	void logMessage(const juce::String& m);
	void addMessageToList(const juce::MidiMessage& message);

	void drawNextLineOfSpectrogram();
	void timerCallback() override;

    void findRangeValueFunction(float* fftData, int index);
    void scaleFunction(float* data, int index);

	int velocityRange(float energyAmount);
    
    //void findRangeValueFunction(float *data);
    //void scaleFunction(float min,float max,float* data);
    
	//Beat detection graphics methods=========================
    void BPMDetection(double timeNow);
    void manualBPM();

	int setLightNumber(int index, int limit);
    void designLightPattern();
    void showConnectionErrorMessage(const juce::String&);
    
    
    void refreshBPMcalculation();

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	PluginProcessor& processor;
    
	juce::TextEditor midiMessagesBox;
	juce::TextEditor actualBPM;
	juce::TextEditor actualVar;
	juce::TextEditor minimumVar;
	juce::TextEditor transientAttack;
	juce::TextEditor panCount;
	juce::TextEditor spectralCount;
	juce::TextEditor velocitySubBar;
	juce::TextEditor velocityMessage;
	
	//int countVelMess = 0;

    juce::TextButton tapTempo;
    juce::TextButton manualMode;
    juce::TextButton resetVarianceBeat;
    
    bool onOff = false;

	int midiChannel = 8;
	double startTime;

	std::unique_ptr<juce::MidiOutput> midiOutput;
    //MidiOutput* midiOutput;

	juce::dsp::FFT forwardFFT;            // [3]
	juce::Image spectrogramImage;

    OSCSender oscSender;
	
    int countNoUpdateVar = 0; //contatore per aggiornare min varianza
    
    int count_beat = 0;
    float maxThresh = 0;
    
	double BPMsum = 0;
	double BPMsumq = 0;
	double varianceBeat = 0.01; 
	int numBeat = 0;
	int numBeatSize = 10;
    int BPM = 0;
    int prevBPM=0;
	double prevTime = 0;
	double prevTimeTemp = 0;
	std::queue<double> deltaTQueue;     //each value in the queue is the difference of consecutive times where a beat is detected.
    
    double var = 0;
    
    double timeAverage = 0; //manual mode

    float minAbs = 10000; //per assegnarli con sicurezza al primo ciclo
    float maxAbs = -10000;
    
    PanFeature panFeature;
    BeatDetector beatDetector;
    SpectralCentroid spectralCentroid;
    
	juce::Slider midiChannelSelector;
    juce::Slider numAnimSelector;
	juce::Slider bpmMaxSelector;
    
	float bpmMax = 180;

	float numAnimazioni = 7;
	float spectralRange = 5; //1.5 - (-3.5)
	float spectralRangeMin = -3.5;
	int repetition = 0;
	int lightNumber = 11; //inizializzato solo per ragioni di sicurezza

    int countBPMRefresh=1;
    
    int queueSize = 12;
    std::queue<float> velocityQueue;
    std::queue<float> temporalVelocityQueue;
    float averageVelocity=0;
    
    void fillVelocityQueue(float);
    
    juce::Label ipTextField;
    juce::Label ipText;
    juce::TextButton button0;
    juce::String ipString;
    bool connectedOSC = false;
    
	float minVelocity = std::numeric_limits<float>::max(); //per assegnarli con sicurezza al primo ciclo
	float maxVelocity = std::numeric_limits<float>::min();
    
    float secondsBeforeRefreshBPM = 3;
    
    float prev_time_bpm=0;   //var usata per fare il check se non mando nuovi bpm entro un certo tempo
    void changeProcessingIPAddress();
    
    float prevMinimumVar = 50;
    int countNewQueue = 0;
    void createComparisonBPMQueue(double timeNow, int roundedIndexNewQueue, double deltaT);
    std::queue<double> comparisonDeltaTQueue;
    double comparisonBPMsum = 0;
    double comparisonBPMsumq = 0;
    double comparisonVar = 0;
    double comparisonAv = 0;
    
    int numComparisonBeatSize = 5;

    int bpmOffsetValue = 3;
    
    int secondsDurationTransientPhase = 10;
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
