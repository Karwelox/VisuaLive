/*
==============================================================================

This file was auto-generated!

It contains the basic framework code for a JUCE plugin editor.

==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p)
	: AudioProcessorEditor(&p), processor(p), startTime(Time::getMillisecondCounterHiRes() * 0.001),
	forwardFFT(PluginProcessor::fftOrder), spectrogramImage(Image::RGB, 512, 512, true), panFeature(processor), beatDetector(processor), spectralCentroid(processor)
{
    panFeature.addChangeListener(this);
    beatDetector.addChangeListener(this);
    spectralCentroid.addChangeListener(this);
    
	processor.addChangeListener(this);

    StringArray midiOutputList = MidiOutput::getDevices();
    int portIndex = midiOutputList.indexOf("loopMIDI Port");
    
    if (portIndex != -1)
        midiOutput = MidiOutput::openDevice(portIndex); //WINDOWS
    else
        midiOutput = MidiOutput::openDevice(0); //MAC


	addAndMakeVisible(panCount);
	panCount.setText("panCount: calculate...");
	panCount.setReadOnly(true);
	panCount.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	panCount.setColour(TextEditor::shadowColourId, Colour(0x16000000));


	addAndMakeVisible(spectralCount);
	spectralCount.setText("spectralCount: calculate...");
	spectralCount.setReadOnly(true);
	spectralCount.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	spectralCount.setColour(TextEditor::shadowColourId, Colour(0x16000000));

	addAndMakeVisible(velocitySubBar);
	velocitySubBar.setReadOnly(true);
	velocitySubBar.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	velocitySubBar.setColour(TextEditor::shadowColourId, Colour(0x16000000));
	velocitySubBar.setText("Velocity: ");
	velocitySubBar.setJustification(4);

	addAndMakeVisible(velocityMessage);
	//velocityMessage.setText("velocityMessage: calculate...");
	velocityMessage.setReadOnly(true);
	velocityMessage.setColour(TextEditor::backgroundColourId, Colours::greenyellow);

	addAndMakeVisible(actualBPM);
	actualBPM.setText("BPM: calculate...");
	actualBPM.setReadOnly(true);
	actualBPM.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	actualBPM.setColour(TextEditor::shadowColourId, Colour(0x16000000));

    addAndMakeVisible(tapTempo);
    tapTempo.setButtonText("Tap Tempo"); 
	tapTempo.setColour(TextButton::buttonColourId, Colour(0x32ffffff));
    tapTempo.onClick = [this] { if(onOff) manualBPM(); };

	addAndMakeVisible(resetVarianceBeat);
	resetVarianceBeat.setButtonText("BPM Recalculate");
	resetVarianceBeat.setColour(TextButton::buttonColourId, Colour(0x32ffffff));
	resetVarianceBeat.onClick = [this] { varianceBeat = 50;
        minimumVar.setText("minimumVar: " + (String)varianceBeat);
    };

    addAndMakeVisible(manualMode);
    manualMode.setButtonText("Manual Mode");
	manualMode.setColour(TextButton::buttonColourId, Colour(0x32ffffff));
	manualMode.setColour(TextButton::buttonOnColourId, Colour(0x32ffffff));
	manualMode.setColour(TextButton::textColourOnId, Colours::greenyellow);
	manualMode.setClickingTogglesState(true);
    manualMode.onClick = [this] {
        onOff = !onOff;
        if(onOff) {
            numBeat = 0;
            timeAverage = 0;
            //manualMode.setButtonText("Manual Mode: on");
			if (beatDetector.transient) {
				minVelocity = std::numeric_limits<float>::max();
				maxVelocity = std::numeric_limits<float>::min();
			}
			beatDetector.beforeTransient = false;
			beatDetector.transient = false;
			beatDetector.reducedTransient = false;
			transientAttack.applyColourToAllText(Colours::white);
        }
        else {
            BPMsum = 0;
            BPMsumq = 0;
            varianceBeat = 50; //alta all'inizio
            numBeat = 0;
            prevTime = 0;
			prevTimeTemp = 0;
            while(!deltaTQueue.empty()) {
                deltaTQueue.pop();
            }
            //manualMode.setButtonText("Manual Mode: off");
        }
    };

	addAndMakeVisible(actualVar);
	addAndMakeVisible(minimumVar);
	addAndMakeVisible(transientAttack);
	actualVar.setText("actualVar: calculate...");
	minimumVar.setText("minimumVar: calculate...");
	transientAttack.setText("transientAttack");
	actualVar.setReadOnly(true);
	minimumVar.setReadOnly(true);
	transientAttack.setReadOnly(true);
	actualVar.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	actualVar.setColour(TextEditor::shadowColourId, Colour(0x16000000));
	minimumVar.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	minimumVar.setColour(TextEditor::shadowColourId, Colour(0x16000000));
	transientAttack.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	transientAttack.setColour(TextEditor::shadowColourId, Colour(0x16000000));

	addAndMakeVisible(midiMessagesBox);
	midiMessagesBox.setMultiLine(true);
	midiMessagesBox.setReturnKeyStartsNewLine(true);
	midiMessagesBox.setReadOnly(true);
	midiMessagesBox.setScrollbarsShown(true);
	midiMessagesBox.setCaretVisible(false);
	midiMessagesBox.setPopupMenuEnabled(true);
	midiMessagesBox.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
	//midiMessagesBox.setColour(TextEditor::outlineColourId, Colour(0x1c000000));
	midiMessagesBox.setColour(TextEditor::shadowColourId, Colour(0x16000000));
	getLookAndFeel().setColour(ScrollBar::thumbColourId, Colours::greenyellow);

	startTimerHz(60);
	setSize(860, 450);

	addAndMakeVisible(midiChannelSelector);
	midiChannelSelector.setRange(1, 16, 1);
	midiChannelSelector.setTextValueSuffix(" Ch");
	midiChannelSelector.setValue(midiChannel);
	midiChannelSelector.addListener(this);

	addAndMakeVisible(bpmMaxSelector);
	bpmMaxSelector.setRange(1, 200, 1);
	bpmMaxSelector.setTextValueSuffix(" BPM");
	bpmMaxSelector.setValue(bpmMax);
	bpmMaxSelector.addListener(this);

	/*addAndMakeVisible(numAnimSelector);
	numAnimSelector.setRange(3, 42, 1);
	numAnimSelector.setTextValueSuffix(" N");
	numAnimSelector.setValue(numAnimazioni);
	numAnimSelector.addListener(this);*/
    
    addAndMakeVisible(ipTextField);
    ipTextField.setText ("Processing IP: ", dontSendNotification);
    
    addAndMakeVisible(ipText);
    ipText.setEditable (true);
    ipText.setText ("192.168.1.4", dontSendNotification);
    ipText.setColour (Label::outlineColourId , Colours::white);
    ipText.setColour (Label::outlineWhenEditingColourId, Colours::greenyellow);
    

	getLookAndFeel().setColour(Slider::thumbColourId, Colours::greenyellow);
    
    
    addAndMakeVisible(button0);
    button0.setButtonText((String)("Connect"));
    button0.onClick = [this] { changeProcessingIPAddress(); };

    
    
    
    //OSC CONNECTION
    if (! oscSender.connect ("192.168.1.3", 12001)){ // [4]
        showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");
        
    }
    
   
    
    
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint(Graphics& g)
{
	//g.fillAll(Colours::black);
	//g.setOpacity(1.0f);
	//g.drawImage(spectrogramImage, getLocalBounds().toFloat());

}

void PluginEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	auto halfWidth = getWidth() / 2;

	auto buttonsBounds = getLocalBounds().withWidth(halfWidth).reduced(10);

	actualBPM.setBounds(buttonsBounds.getX(), 10, buttonsBounds.getWidth(), 20);

	midiMessagesBox.setBounds(getLocalBounds().withWidth(halfWidth).withX(halfWidth).reduced(10));
    
    tapTempo.setBounds(buttonsBounds.getX(), 230, buttonsBounds.getWidth(), 20);
    
    manualMode.setBounds(buttonsBounds.getX(), 260, buttonsBounds.getWidth(), 20);

	resetVarianceBeat.setBounds(buttonsBounds.getX(), 290, buttonsBounds.getWidth(), 20);

	midiChannelSelector.setBounds(buttonsBounds.getX(), 330, buttonsBounds.getWidth(), 20);

	bpmMaxSelector.setBounds(buttonsBounds.getX(), 360, buttonsBounds.getWidth(), 20);

	/*numAnimSelector.setBounds(buttonsBounds.getX(), 390, buttonsBounds.getWidth(), 20);*/
    
    
  
	
	actualVar.setBounds(buttonsBounds.getX(), 40, buttonsBounds.getWidth(), 20);

	minimumVar.setBounds(buttonsBounds.getX(), 70, buttonsBounds.getWidth(), 20);

	transientAttack.setBounds(buttonsBounds.getX(), 100, buttonsBounds.getWidth(), 20);

	panCount.setBounds(buttonsBounds.getX(), 130, buttonsBounds.getWidth(), 20);

	spectralCount.setBounds(buttonsBounds.getX(), 160, buttonsBounds.getWidth(), 20);

	velocitySubBar.setBounds(buttonsBounds.getX(), 190, buttonsBounds.getWidth(), 20);
    
	velocityMessage.setBounds(buttonsBounds.getX(), 190, buttonsBounds.getWidth(), 20);

    ipTextField.setBounds(buttonsBounds.getX(), 390, buttonsBounds.getWidth(),20);
    
    ipText.setBounds(buttonsBounds.getX()+100, 390, 200,20);
    
    button0.setBounds(buttonsBounds.getX()+315, 390, 80, 20);
    
}

//MIDI==========================================================================
static String getMidiMessageDescription(const MidiMessage& m)
{
	if (m.isNoteOn())           return "Note on " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isProgramChange())    return "Program change " + String(m.getProgramChangeNumber());
	if (m.isPitchWheel())       return "Pitch wheel " + String(m.getPitchWheelValue());
	if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + String(m.getAfterTouchValue());
	if (m.isChannelPressure())  return "Channel pressure " + String(m.getChannelPressureValue());
	if (m.isAllNotesOff())      return "All notes off";
	if (m.isAllSoundOff())      return "All sound off";
	if (m.isMetaEvent())        return "Meta event";

	if (m.isController())
	{
		String name(MidiMessage::getControllerName(m.getControllerNumber()));

		if (name.isEmpty())
			name = "[" + String(m.getControllerNumber()) + "]";

		return "Controller " + name/* + ": " + String(m.getControllerValue())*/;
	}

	return String::toHexString(m.getRawData(), m.getRawDataSize());
}

void PluginEditor::setNoteNumber(int faderNumber, int velocity)
{
    //auto message = MidiMessage::noteOn(midiChannel, noteNumber, (uint8)100);
    auto message = MidiMessage::controllerEvent(midiChannel, faderNumber, velocity);
    
    double timeNow = Time::getMillisecondCounterHiRes() * 0.001;
    
	if (faderNumber == 0) {
		message.setTimeStamp(timeNow - startTime);
		midiOutput->sendMessageNow(message);
		//printf("\n%f", spectralCentroid.centroidL);

		//velocityMessage.setText((String)maxVelocity);
		velocityMessage.setBounds(getLocalBounds().withWidth(getWidth() / 2).reduced(10).getX(), 190, ((velocity * getLocalBounds().withWidth(getWidth() / 2).reduced(10).getWidth()) / 127), 20);
		velocitySubBar.setText("Energy: " + (String)velocity);
		//countVelMess++;
        
        //creo la coda di velocity
        if(velocityQueue.size()<queueSize){  //solo per le prime 10 iterazioni
            velocityQueue.push(velocity);
        }
        
        else{    //altrimenti elimino elemento più vecchio, aggiungo il più nuovo e calcolo la media
            velocityQueue.pop();
            velocityQueue.push(velocity);
            fillVelocityQueue(velocity);
        }
        
        
    
        
        if(connectedOSC){
            if (! oscSender.send ("/juce/velocity", (float)averageVelocity)){ // [5]
                showConnectionErrorMessage ("Error: could not send OSC message. \n DISCONNECTED.");
                oscSender.disconnect();
                printf("DISCONNESSO");
                connectedOSC = false;
            }
        }
        
        
        
        if(connectedOSC){
        if (! oscSender.send ("/juce/centroid", spectralCentroid.averageCentroid)){
            showConnectionErrorMessage ("Error: could not send OSC message.");
        }
        }
        
        if(connectedOSC){
            if (! oscSender.send ("/juce/continuePanning", panFeature.panValue)){ // [5]
                showConnectionErrorMessage ("Error: could not send OSC message.");
                
            }
        }
        
	}

	else if (timeNow - prevTime - startTime > 0.3 && !onOff)   //200bpm massimi
	{
		//BPMDetection(timeNow);
		//prevTime = timeNow - startTime; //occhio
        
        //prev_time_bpm = timeNow;   //variabile usata per il refresh del bpm
        
		if (timeNow - prevTimeTemp - startTime > (60 / bpmMax))
		{
            BPMDetection(timeNow);
            prevTime = timeNow - startTime; //occhio
            
            prev_time_bpm = timeNow;   //variabile usata per il refresh del bpm
            
            
			prevTimeTemp = timeNow - startTime;
			lightNumber = setLightNumber(faderNumber, velocity);
			
			message = MidiMessage::controllerEvent(midiChannel, lightNumber, velocity);
			message.setTimeStamp(timeNow - startTime);
			midiOutput->sendMessageNow(message);
			addMessageToList(message);
			
            
            if(connectedOSC){
            if (! oscSender.send ("/juce/beat", (float)1)){ // [5]
                showConnectionErrorMessage ("Error: could not send OSC message.");
            }
            }
            //invio del valore di panning
            /*if (! oscSender.send ("/juce/panning", panFeature.panValue)) // [5]
                showConnectionErrorMessage ("Error: could not send OSC message.");*/
            
            if(connectedOSC){
            if (! oscSender.send ("/juce/panning", panFeature.panValue)){ // [5]
                showConnectionErrorMessage ("Error: could not send OSC message.");

            }
            }
            countBPMRefresh=1;   //restart counter bpm refresh
            
		}
	}

	else if (onOff)
	{
		lightNumber = setLightNumber(faderNumber, velocity);

		message = MidiMessage::controllerEvent(midiChannel, lightNumber, velocity);
		message.setTimeStamp(timeNow - startTime);
		midiOutput->sendMessageNow(message);
		addMessageToList(message);
	}
   
}

void PluginEditor::BPMDetection(double timeNow)
{
	if (beatDetector.transient)
	{
		transientAttack.applyColourToAllText(Colours::greenyellow);
		if (timeNow - beatDetector.transientStartTime > 10)
		{
			beatDetector.transient = false;
			minVelocity = std::numeric_limits<float>::max();
			maxVelocity = std::numeric_limits<float>::min();
			transientAttack.applyColourToAllText(Colours::white);
		}
		if (beatDetector.reducedTransient)
		{
			beatDetector.reducedTransient = false;
			minVelocity = std::numeric_limits<float>::max();
			maxVelocity = std::numeric_limits<float>::min();
		}
	}

	else 
	{
		double deltaT = timeNow - prevTime - startTime;
		deltaTQueue.push(deltaT);

		numBeat++;

		//prevTime = timeNow - startTime;  //messo fuori, vediamo se è meglio

		BPMsum = BPMsum + deltaT;
		BPMsumq = BPMsumq + (deltaT * deltaT);

		if (numBeat >= numBeatSize)
		{
               //nuova variabile
            int roundedIndexNewQueue = countNewQueue%numComparisonBeatSize;   //perchè voglio avere nuove code ogni beatSize
            
            createComparisonBPMQueue(timeNow, roundedIndexNewQueue, deltaT);
            
            countNewQueue++;
            
            
            
			double av = BPMsum / (numBeatSize);
			var = BPMsumq / (numBeatSize) - (av * av);
			//double var = BPMsumq / (numBeatSize-1) + (av * av * numBeatSize) / (numBeatSize - 1) - (2 * av * BPMsum) / (numBeatSize - 1);
            
            
            
            
            
			if (var < varianceBeat)
			{
                
                prevBPM = BPM;
                
                int calcolusBPM = round(60 / av);
                
                minVelocity = std::numeric_limits<float>::max(); //per assegnarli con sicurezza al primo ciclo
                maxVelocity = std::numeric_limits<float>::min();
                
                
                if(calcolusBPM>prevBPM+bpmOffsetValue || calcolusBPM<prevBPM-bpmOffsetValue){   //controllo per evitare di avere variazioni minime di calcolo di BPM
                    BPM = calcolusBPM;
                    //printf("\nUPDATE!! ActualBPM = %d  |  PrevBPM = %d", prevBPM, BPM);
                
                    
                    if(connectedOSC){
                        if (! oscSender.send ("/juce/bpm", BPM)) // [5]
                            showConnectionErrorMessage ("Error: could not send OSC message.");
                        }
                    
                    varianceBeat = var;
                    actualBPM.setText("BPM: " + (String)BPM);
                    
                    printf("\nCODA SUPREMA. BPM attuale: %d  || Varianza attuale: %f", BPM, var);
                    
                }
                
            else{
                        //printf("\nNO update! ActualBPM = %d  |  PrevBPM = %d", prevBPM, BPM);
                    }
                
				
                
                
			}
            
            /*countNoUpdateVar++;      //prova per far recepire prima il cambio bpm
            if (countNoUpdateVar>5){
                varianceBeat+=0.0001;
                countNoUpdateVar=0;
                
            }*/
            
			actualVar.setText("actualVar: " + (String)var);
			minimumVar.setText("minimumVar: " + (String)varianceBeat);

			BPMsum = BPMsum - deltaTQueue.front();
			BPMsumq = BPMsumq - (deltaTQueue.front() * deltaTQueue.front());
			deltaTQueue.pop();
			numBeat--;

		}
	}//fine caso fuori dal transient iniziale
}

void PluginEditor::manualBPM()
{
    double timeNow = Time::getMillisecondCounterHiRes() * 0.001;
    //auto message = MidiMessage::controllerEvent(midiChannel, 7, ((int)(Time::getMillisecondCounterHiRes()) % 128));
	designLightPattern();

    numBeat++;
    
    if(numBeat > 1)
        timeAverage = (timeAverage * (numBeat - 2) + (timeNow - prevTime - startTime)) / (numBeat - 1);
    
    prevTime = timeNow - startTime;
    
    if(numBeat > 1) {
        BPM = round(60 / timeAverage);
		actualBPM.setText("BPM: " + (String)BPM);
        
        
        if(connectedOSC){
        if (! oscSender.send ("/juce/bpm", (float)BPM)) // [5]
            showConnectionErrorMessage ("Error: could not send OSC message.");
        }
    }
    
   /* message.setTimeStamp((timeNow - startTime));
    midiOutput->sendMessageNow(message);
    addMessageToList(message);*/
}

void PluginEditor::logMessage(const String& m)
{
	midiMessagesBox.moveCaretToEnd();
	midiMessagesBox.insertTextAtCaret(m + newLine);
}

void PluginEditor::addMessageToList(const MidiMessage& message)
{
	auto time = message.getTimeStamp();

	auto hours = ((int)(time / 3600.0)) % 24;
	auto minutes = ((int)(time / 60.0)) % 60;
	auto seconds = ((int)time) % 60;
	auto millis = ((int)(time * 1000.0)) % 1000;

	auto timecode = String::formatted("%02d:%02d:%02d.%03d",
		hours,
		minutes,
		seconds,
		millis);

	if (lightNumber < 10)
		logMessage(timecode + " - |0" + (String)lightNumber + "|" + " " + getMidiMessageDescription(message));
	else
		logMessage(timecode + " - |" + (String)lightNumber + "|" + " " + getMidiMessageDescription(message));
}

void PluginEditor::sliderValueChanged(Slider * slider)
{
	/*if (slider == &thresholdSlider) {
		processor.setThreshold(slider->getValue());
	}*/  //Potrebbe servire

	if (slider == &midiChannelSelector) {
		midiChannel = slider->getValue();
		//tapTempo.setButtonText((String)midiChannel);
	}

	if (slider == &bpmMaxSelector) {
		bpmMax = slider->getValue();
		//tapTempo.setButtonText((String)bpmMax);
	}

	/*if (slider == &numAnimSelector) {
		numAnimazioni = slider->getValue();
		//tapTempo.setButtonText((String)numAnimazioni);
	}*/
}

void PluginEditor::changeListenerCallback(ChangeBroadcaster* source)
{

    /*if(source == &panFeature )
    {
        //END OF THE PAN FEATURE THREAD (to be handle)
        //printf("\n%d", panFeature.panValue);
        
    }
    
    else if(source == &beatDetector)
    {
        //END OF THE BEAT DETECTION THREAD
        //setNoteNumber(beatDetector.beatTime);
        
    }
    
    else if(source == &spectralCentroid)
    {
        //END OF THE SPECTRAL CENTROID THREAD (to be handle)
        //printf("\n%f", spectralCentroid.centroidL);

    }*/
	designLightPattern();
    
   
    
    
    
}


void PluginEditor::drawNextLineOfSpectrogram()
{
	auto rightHandEdge = spectrogramImage.getWidth() - 1;
	auto imageHeight = spectrogramImage.getHeight();
	spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);                       // [1]
    
	forwardFFT.performFrequencyOnlyForwardTransform(processor.fftDataL);                         // [2]
	forwardFFT.performFrequencyOnlyForwardTransform(processor.fftDataR);
    
	float fftData[PluginProcessor::fftSize / 2];

	for (int i = 0; i <= processor.fftSize / 2; i++) {
		fftData[i] = (processor.fftDataL[i] + processor.fftDataR[i])/2;
        processor.newFftData[i] = fftData[i];
        //printf("%f ", processor.newFftData[i]);
	}
    //printf("\n\n");
    
    

	auto maxLevel = FloatVectorOperations::findMinAndMax(fftData, processor.fftSize / 2); // [3]
	
	for (auto y = 1; y < imageHeight; ++y)                                                            // [4]
	{
		auto skewedProportionY = 1.0f - std::exp(std::log(y / (float)imageHeight) * 0.2f);
		auto fftDataIndex = jlimit(0, processor.fftSize / 2, (int)(skewedProportionY * processor.fftSize / 2));
		auto level = jmap(processor.fftDataL[fftDataIndex] + processor.fftDataR[fftDataIndex], 0.0f, jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);
		spectrogramImage.setPixelAt(rightHandEdge, y, Colour::fromHSV(level, 1.0f, level, 1.0f));   // [5]
	}

    findRangeValueFunction(processor.fftDataL, 0);
	findRangeValueFunction(processor.fftDataR, 1);

    
	
    
    if(!onOff)
        beatDetector.run();         //START BEAT DETECTION THREAD
    
    panFeature.run();               //START PAN FEATURE THREAD
    
    spectralCentroid.run();         //START SPECTRAL CENTROID THREAD
    

	//-----FORSE
	float energySum = beatDetector.performEnergyFFT(2);
    if(connectedOSC)     //solo se effettivamente connetto inizio ad aggiornare l' energia
        setNoteNumber(0, velocityRange(10*log10(1+energySum + std::numeric_limits<float>::epsilon())));
    
    double time_now = Time::getMillisecondCounterHiRes() * 0.001;
    
    //AGGIORNO IL CALCOLO DEL BPM SE NON ARRIVA UN BEAT ENTRO secondsBeforeRefreshBPM SECONDI
    if(time_now-prev_time_bpm>secondsBeforeRefreshBPM){     //secondsBeforeRefreshBPM è il num di secondi di attesa se non arriva un beat allora refresho
        //printf("%f  -   %f\n", time_now,prev_time_bpm);
        prev_time_bpm = time_now;
        //refreshBPMcalculation();
        
        countBPMRefresh++;
    }

    
    

}

//scala il valore dell energia in ingresso tra il max e min valore trovato nella storia e restituisce un valore tra 0 e 127 (velocity)
int PluginEditor::velocityRange(float energyAmount) {
    
    
	if (energyAmount > maxVelocity/* || countVelMess > 18 * 20*/) //18: numero di messaggi al secondo circa
		maxVelocity = energyAmount;
	if (energyAmount < minVelocity/* || countVelMess > 18 * 20*/)
		minVelocity = energyAmount;
	//if (countVelMess > 18 * 20) //ogni 20 secondi di musica circa ricalcolo max e min
		//countVelMess = 0;

	float ris;
    
	if (maxVelocity - minVelocity > 0) {
		ris = (int)(127 * ((energyAmount - minVelocity) / (maxVelocity - minVelocity)) - 0);
		if (ris > 127)
			ris = 127;
		return ris;
	}
	else
		return 0;
    


}

void PluginEditor::timerCallback()
{
	if (beatDetector.beforeTransient) {
		transientAttack.applyColourToAllText(Colours::white);
	}

	if (processor.getNextFFTBlockReady())
	{
		drawNextLineOfSpectrogram();
		processor.setNextFFTBlockReady(false);
		repaint();
	}
}

void PluginEditor::findRangeValueFunction(float* data, int index)
{
    //float* data = processor.getFFTData();

    for(int i = 0; i < PluginProcessor::fftSize / 2; i++) {
        if(data[i] > maxAbs)
            maxAbs = data[i];
        if(data[i] < minAbs)
            minAbs = data[i];
    }
    
    scaleFunction(data, index);
   
}

void PluginEditor::scaleFunction(float* data, int index)
{
    if(index == 0 && maxAbs - minAbs != 0)
		for(int i=0; i < PluginProcessor::fftSize; i++){
			//processor.getFFTData()[i] =  ((data[i]-min) * (1-(-1))) / ((max-min)+(-1))  ;
			processor.fftDataL[i] = 1 *((data[i] - minAbs) / (maxAbs - minAbs)) - 0;
		}
	else if(index == 1 && maxAbs - minAbs != 0)
		for (int i = 0; i < PluginProcessor::fftSize; i++) {
			//processor.getFFTData()[i] =  ((data[i]-min) * (1-(-1))) / ((max-min)+(-1))  ;
			processor.fftDataR[i] = 1 * ((data[i] - minAbs) / (maxAbs - minAbs)) - 0;
		}
    
}

int PluginEditor::setLightNumber(int index, int limit)
{
	if (repetition == index)
	{
		if (repetition >= limit) {
			repetition = index - 1;
		}
		else
		{
			repetition = index + 1;
		}
	}
	else
	{
		repetition = index;
	}

	//tapTempo.setButtonText((String)repetition);
	return repetition;

}

void PluginEditor::designLightPattern()
{
    
	for (int i = 0; i < numAnimazioni; i++) {
		if (panFeature.panValue < -0.5) {   //+-0.5 va bene per il panning? io metterei di più, tipo +-0.8
			if (spectralRangeMin <= -3.5) {
				if (spectralCentroid.centroidL <= -3.5)
				{
					setNoteNumber(i + 1, numAnimazioni); //velocity usata come limite nel setLightNumber
					break;
				}
			}
			else if(spectralRangeMin > 1.5) {
				if (spectralCentroid.centroidL > 1.5)
				{
					setNoteNumber(i + 1, numAnimazioni);
					break;
				}
			}
			else {
				if (spectralCentroid.centroidL > spectralRangeMin - (spectralRange / (numAnimazioni - 2)) && spectralCentroid.centroidL <= spectralRangeMin)
				{
					setNoteNumber(i + 1, numAnimazioni);
					break;
				}
			}
		}

		else if (panFeature.panValue > 0.5) {
			if (spectralRangeMin <= -3.5) {
				if (spectralCentroid.centroidR <= -3.5)
				{
					setNoteNumber(i + 1 + numAnimazioni * 2, numAnimazioni * 3);
					break;
				}
			}
			else if (spectralRangeMin > 1.5) {
				if (spectralCentroid.centroidR > 1.5)
				{
					setNoteNumber(i + 1 + numAnimazioni * 2, numAnimazioni * 3);
					break;
				}
			}
			else {
				if (spectralCentroid.centroidR > spectralRangeMin - (spectralRange / (numAnimazioni - 2)) && spectralCentroid.centroidR <= spectralRangeMin)
				{
					setNoteNumber(i + 1 + numAnimazioni * 2, numAnimazioni * 3);
					break;
				}
			}
		}

		else {
			if (spectralRangeMin <= -3.5) {
				if (spectralCentroid.centroidMid <= -3.5)
				{
					setNoteNumber(i + 1 + numAnimazioni, numAnimazioni * 2);
					break;
				}
			}
			else if (spectralRangeMin > 1.5) {
				if (spectralCentroid.centroidMid > 1.5)
				{
					setNoteNumber(i + 1 + numAnimazioni, numAnimazioni * 2);
					break;
				}
			}
			else {
				if (spectralCentroid.centroidMid > spectralRangeMin - (spectralRange / (numAnimazioni - 2)) && spectralCentroid.centroidMid <= spectralRangeMin)
				{
					setNoteNumber(i + 1 + numAnimazioni, numAnimazioni * 2);
					break;
				}
			}
		}

		spectralRangeMin += (spectralRange / (numAnimazioni - 2));
	}

	spectralRangeMin = -3.5;

    
    
	panCount.setText("Panning: " + (String)panFeature.panValue);
	spectralCount.setText("SpectralCentroidMid: " + (String)spectralCentroid.centroidMid);
}



void PluginEditor::showConnectionErrorMessage (const String& messageText)
{
    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                      "Connection error",
                                      messageText,
                                      "OK");
    }


void PluginEditor::refreshBPMcalculation(){
    
    varianceBeat = 50;
    var = 0.05;
    minimumVar.setText("minimumVar: " + (String)varianceBeat);
    actualVar.setText("actualVar: " + (String)var);
    
}



void PluginEditor::fillVelocityQueue(float velocity){
    
    float val=0, sum=0;
    
    
    
    temporalVelocityQueue = velocityQueue;
    
    
    //printf("\n");
    while(temporalVelocityQueue.size() > 0)
    {
        val = temporalVelocityQueue.front();
        //printf("%f  ", val);
        sum = sum + val;
        temporalVelocityQueue.pop();
    }
    //printf("\n");
    
    averageVelocity = (sum+0.00000001) / queueSize;
    //printf("\n %f", averageVelocity);
    
    
    
    
}


void PluginEditor::changeProcessingIPAddress(){
    
    ipString = ipText.getText();
    
    if (! oscSender.connect (ipString, 12001)) // [4]
        showConnectionErrorMessage ("Error: could not connect to UDP port 12001.");
    
    else
        juce::NativeMessageBox::showMessageBox(AlertWindow::InfoIcon, "", "Connected with the selected IP");
    
    connectedOSC = true;
}




void PluginEditor::createComparisonBPMQueue(double timeNow, int roundedIndexNewQueue, double deltaT){
    
    
    if (roundedIndexNewQueue<numComparisonBeatSize-1){   //caso in cui sto ricreando una coda temporanea
        
        comparisonDeltaTQueue.push(timeNow);
        comparisonBPMsum = BPMsum + deltaT;
        comparisonBPMsumq = BPMsumq + (deltaT * deltaT);
        comparisonAv = BPMsum / (numBeatSize);
        comparisonVar = BPMsumq / (numBeatSize) - (comparisonAv * comparisonAv);
        //printf("\nDIM attuale: %d", comparisonDeltaTQueue.size());
    }
    
    
    else{  //caso in cui inizializzo la nuova coda
        //CONFRONTO
        int comparisonBPM = round(60 / comparisonAv);
        
        
        //printf("\nBPM attuale: %d  || Varianza attuale: %f", comparisonBPM, comparisonVar);
        
        //se la varianza della coda attuale è minore di una soglia predefinita:
        //1) aggiorno la varianza minima del sistema
        //2) aggiorno il valore del bpm
        if((comparisonVar < 0.001  && (comparisonBPM>prevBPM+bpmOffsetValue || comparisonBPM<prevBPM-bpmOffsetValue))
           || (comparisonVar < 0.0025  && (comparisonBPM>prevBPM+bpmOffsetValue+3 || comparisonBPM<prevBPM-bpmOffsetValue-3))
           )
        {

            
            
            
            if(comparisonVar < 0.001  && (comparisonBPM>prevBPM+bpmOffsetValue || comparisonBPM<prevBPM-bpmOffsetValue)){
                printf("\nCASO 1: BPM attuale: %d  || Varianza attuale: %f", comparisonBPM, comparisonVar);
            }
            if(comparisonVar < 0.0025  && (comparisonBPM>prevBPM+bpmOffsetValue+3 || comparisonBPM<prevBPM-bpmOffsetValue-3)){
                printf("\nCASO 2: BPM attuale: %d  || Varianza attuale: %f", comparisonBPM, comparisonVar);
            }
            
            
            
            prevBPM = BPM;
            BPM = comparisonBPM;
            
           
            
        
            
            //printf("\nBPM attuale: %d  || Varianza attuale: %f", comparisonBPM, comparisonVar);
            varianceBeat = 0.001;
            actualBPM.setText("BPM: " + (String)BPM);
            
            if(connectedOSC){
                if (! oscSender.send ("/juce/bpm", BPM)) // [5]
                    showConnectionErrorMessage ("Error: could not send OSC message.");
            }
            
               
        }
        
        
        
        //poi ricreo la coda
        
        while(!comparisonDeltaTQueue.empty()) {
            comparisonDeltaTQueue.pop();
        }
        comparisonBPMsum = 0;
        comparisonBPMsumq = 0;
        comparisonAv = 0;
        comparisonVar = 0;
        //printf("\nRICREO: %d", comparisonDeltaTQueue.size());
        
        //metto il primo valore della nuova coda
        comparisonDeltaTQueue.push(timeNow);
        
        //printf("\nDIM attuale: %d", comparisonDeltaTQueue.size());
    }
    
    
    
    
}
