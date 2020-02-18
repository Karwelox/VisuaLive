/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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
class PluginEditor : public AudioProcessorEditor,
	public ChangeListener,
	public Slider::Listener,
	private Timer,
    public OSCSender
    
{
public:
	PluginEditor(PluginProcessor&);
	~PluginEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void changeListenerCallback(ChangeBroadcaster* source) override;
	void sliderValueChanged(Slider* slider) override;

	void setNoteNumber(int faderNumber, int velocity);
	void logMessage(const String& m);
	void addMessageToList(const MidiMessage& message);

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
    void showConnectionErrorMessage(const String&);
    
    
    void refreshBPMcalculation();

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	PluginProcessor& processor;
	TextEditor midiMessagesBox;
    
	TextEditor actualBPM;
	TextEditor actualVar;
	TextEditor minimumVar;
	TextEditor transientAttack;
	TextEditor panCount;
	TextEditor spectralCount;
	TextEditor velocitySubBar;
	TextEditor velocityMessage;
	
	//int countVelMess = 0;

    TextButton tapTempo;
    TextButton manualMode;
    bool onOff = false;

	TextButton resetVarianceBeat;

	int midiChannel = 8;
	double startTime;

	std::unique_ptr<MidiOutput> midiOutput;
    //MidiOutput* midiOutput;

	dsp::FFT forwardFFT;            // [3]
	Image spectrogramImage;

    OSCSender oscSender;
	
    int countNoUpdateVar = 0; //contatore per aggiornare min varianza
    
    int count_beat = 0;
    float maxThresh = 0;
    
	double BPMsum = 0;
	double BPMsumq = 0;
	double varianceBeat = 50; //alta all'inizio
	int numBeat = 0;
	int numBeatSize = 10;
    int BPM = 0;
    int prevBPM=0;
	double prevTime = 0;
	double prevTimeTemp = 0;
	std::queue<double> deltaTQueue;
    
    double var = 0;
    
    double timeAverage = 0; //manual mode

    float minAbs = 10000; //per assegnarli con sicurezza al primo ciclo
    float maxAbs = -10000;
    
    PanFeature panFeature;
    BeatDetector beatDetector;
    SpectralCentroid spectralCentroid;
    
	Slider midiChannelSelector;
	
	Slider bpmMaxSelector;
	float bpmMax = 200;

	Slider numAnimSelector;
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
    
    Label ipTextField;
    Label ipText;
    TextButton button0;
    String ipString;
    Boolean connectedOSC = false;
    
	float minVelocity = std::numeric_limits<float>::max(); //per assegnarli con sicurezza al primo ciclo
	float maxVelocity = std::numeric_limits<float>::min();
    
    float secondsBeforeRefreshBPM = 3;
    
    float prev_time_bpm=0;   //var usata per fare il check se non mando nuovi bpm entro un certo tempo
    
    void changeProcessingIPAddress();
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
