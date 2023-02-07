/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin processor.

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class PluginProcessor : public juce::AudioProcessor,
public juce::ChangeBroadcaster
{
public:
	//==============================================================================
	PluginProcessor();
	~PluginProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================
	double setThreshold(double value);
    
    void setThreadBoolean(bool setup);
	int prova = 10;
    
	enum
	{
		fftOrder = 10,           // [1]
		fftSize = 1 << fftOrder  // [2]     //fftSize = 2^10 = 1024. Number of fft samples.
	};
    
    int winSize = fftSize; //Number of window samples in time domain.

	void pushNextSampleIntoFifo(float sample, int channel) noexcept;

	/*float* getFFTData();
	float getFFTDataIndex(int index);*/

	bool getNextFFTBlockReady();
	void setNextFFTBlockReady(bool setup);

	int numSample = 0;

	float fifoL[fftSize];           // [4]
	float fifoR[fftSize];
	int fifoIndexL = 0;              // [6]
	int fifoIndexR = 0;
	float fftDataL[fftSize*2];    // [5]
	float fftDataR[fftSize*2];
    float fftData[fftSize*2];   //media dei due canali, usata nello spectral centroid
    
    float freqs[fftSize/2];    //frequenza corrispondente al centro del suo BIN di riferimento
    
    float newFftData[fftSize*2];
    
    
    
private:
	double rawVolume = 1;
	double threshold;
	int count = 0;

	bool nextFFTBlockReady = false; // [7]

    float leftChannel[1024];
    float rightChannel[1024];
    float totChannel[1024];
    
    bool threadFlag=true;
    
//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
