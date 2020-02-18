//last version

import oscP5.*;
import netP5.*;
import java.util.Timer;
import java.util.TimerTask;
import controlP5.*;
import peasy.*;
PeasyCam cam;
ControlP5 cp5;

final float timeToWaitOtherBPM = 4.5;    //tempo in secondi dopo il quale è possibile cambiare visual

int currentVisual = 0; //indica la visual attuale
int prevVisual = 0;
int totVisuals=9;      //num totale delle visual

int current_bpm = 120;     //
int prev_bpm = 120;        //valori usati per gestire variazione di detection_bpm 

int counterBPM = 0;        //contatore per algoritmo bpm


float startTime = millis();
float currentTime = millis();
float prevTime=millis();

float detection_motion = 0.001;
int detection_bpm=95; 
float detection_beat;
float detection_velocity;
float detection_panning = 0;
float detection_centroid = 0;
float detection_continuePanning = 0;
float prev_centroid = 0;
float prev_averagePanning = 0;
float prev_velocity = 0;
float prev_continuePanning = 0;

float maxCentroid=0;  //valore usato per mappare il centroid
float currentCentroid=0;     //valore del centroid usato nella gestione delle visual (mappato da -1 a 1)


float currentMotion=1;    
float currentMotionPannel = 1; //valore del motion usato nella gestione delle visual (mappato da -0.05 a 1)
float averageMotion;
int sizeMotionValues=20;

float sumMotion=0;    
float maxMotion=0.01;
int fullMotionValue = 100;

int pred = 0;
int pgreen = 0;
int pblue = 0;
  
ArrayList<Integer> arrayVelocity ;
int sizeArrayVelocity = 40;

OscP5 oscP5;
OscP5 oscP5_carm;
//NetAddress myRemoteLocation_receiveFromTouchDesigner = new NetAddress("192.168.1.12",12000);           //indirizzo di chi manda motion da TouchDesigner
//NetAddress myRemoteLocation_receiveFromJuce = new NetAddress("192.168.1.3",12001);    //indirizzo di chi manda audio features da Juce
//NetAddress myRemoteLocation_sendToTouchDesigner= new NetAddress("192.168.1.12",12002);   //indirizzo touch su cui invio a touch
NetAddress myRemoteLocation_receiveFromTouchDesigner;
NetAddress myRemoteLocation_receiveFromJuce;
NetAddress myRemoteLocation_sendToTouchDesigner;

boolean toogle_beat;   //toggle per gestire l' arriva dei beat. ne arriva uno=falso, il successivo=vero ....

FirstVisual firstVisual;
SecondVisual secondVisual;
ThirdVisual thirdVisual;
MainMenu mainMenu;
InfiniteCircle infiniteCircle;
CubeVisual cubeVisual;
InfiniteTriangleVisual infiniteTriangleVisual;
Attractor attractor;
StarFieldVisual starFieldVisual;
RainVisual rainVisual;
KaleidoscopeVisual kaleidoscopeVisual;

Button playButton;


boolean playButtonClicked = false;


CColor colorBottonPlay;
CColor colorIP;

final Timer timer = new Timer();
boolean hasFinished = true;

boolean toggle_beat;

boolean currentBeat;
boolean fullBar = false;

boolean manualMode = false;

PImage backgroundImage;   //immagine background del main
PImage backgroundRain;


int prevVal = 1;

ArrayList<Float> motionList = new ArrayList<Float>();


ArrayList<Float> panningList = new ArrayList<Float>();
ArrayList<Float> panningContinueList = new ArrayList<Float>();
float averagePanning;
float prev_averageContinuePanning;
float averageContinuePanning;

float scaledVelocity;

int channel = 0;
int pitch = 64;
int velocity = 127;

Textfield ipTextFieldTD;
String ipTextTD;

Textfield ipTextFieldJuce;
String ipTextJuce;

JSONArray rangeBPMValuesJSON;
int numRanges = 5;
int[][] rangeBPMMatrix = new int[numRanges][2];

PGraphics pGraphic;


void setup() {
  //size(300,300);
  fullScreen(P3D);
  background(0);
  //size(1280,720,P3D);
  firstVisual = new FirstVisual();
  secondVisual = new SecondVisual();
  thirdVisual = new ThirdVisual();
  mainMenu = new MainMenu("An interactive visual art generator!");
  infiniteCircle = new InfiniteCircle();
  cubeVisual = new CubeVisual();
  infiniteTriangleVisual = new InfiniteTriangleVisual();
  attractor = new Attractor();
  starFieldVisual = new StarFieldVisual();
  rainVisual = new RainVisual();
  kaleidoscopeVisual = new KaleidoscopeVisual();
  
  toggle_beat=false;
  frameRate(60);
  smooth();
  
  //Inserisci FS.jpg per il fullscreen
  backgroundImage = loadImage("backFS.jpg");
  backgroundRain = loadImage("tumblerino2FS.jpg");
  
  cp5 = new ControlP5(this);
  cp5.setAutoDraw(false);
  //colorBottonPlay = new CColor(#d3ced6, 0xffffffff, #f00511, #000000, 0x000000); //(foreground, background,activation,testo,boh)
  colorBottonPlay = new CColor(#d3ced6, 0x000000, 0xffffffff, 0xffffffff, 0x000000);
  colorIP = new CColor(0xffffffff, 0x000000, #f00511, 0xffffffff, 0xffffffff);  //stroke, interno, colore di quando si clicca, testo sotto, testo dentro
  
  arrayVelocity = new ArrayList<Integer>();
  
  
  cam = new PeasyCam(this, 0, 0, 0, 50);
  //cam.setMinimumDistance(50);
  //cam.setMaximumDistance(500);

  //bottone che serve per avviare il programma
  playButton = cp5.addButton("PLAY")
     .setPosition(width/2-50,600)
     .setSize(100,30)
     .setColor(colorBottonPlay)
     .setFont(mainMenu.f)
     ;
     
  ipTextFieldTD = cp5.addTextfield("Touch Designer IP")
     .setPosition(width/2-100,500)
     .setSize(200,40)
     .setFont(mainMenu.ipFont)
     .setAutoClear(false)
     .setColor(colorIP)
     .setText("192.168.1.3")
     ;
     
  ipTextFieldJuce = cp5.addTextfield("Juce IP")
     .setPosition(width/2-100,420)
     .setSize(200,40)
     .setFont(mainMenu.ipFont)
     .setAutoClear(false)
     .setColor(colorIP)
     .setText("192.168.1.2")
     ;


  rangeBPMValuesJSON = loadJSONArray("jsonFile.json");

  for (int i = 0; i < rangeBPMValuesJSON.size(); i++) {
    
    JSONObject rangeBPMValues = rangeBPMValuesJSON.getJSONObject(i); 

    rangeBPMMatrix[i][0] = rangeBPMValues.getInt("lower");
    rangeBPMMatrix[i][1] = rangeBPMValues.getInt("upper");
  }
  
  for (int i = 0; i < rangeBPMValuesJSON.size(); i++){
    println(rangeBPMMatrix[i][0] + "  " + rangeBPMMatrix[i][1]);
  }
  
  pGraphic = createGraphics(60, 150, P3D);
  
}




void draw() {
  
  currentTime = millis();
  
  
  
  
  switch(currentVisual) {
  case 0: 
    //cam.beginHUD();

    hint(DISABLE_DEPTH_TEST);
    cam.beginHUD();
    mainMenu.drawText();
    mainMenu.drawPlayButton();
    cp5.draw();
    cam.endHUD();
    hint(ENABLE_DEPTH_TEST);

    break;
    
  case 1:
    cam.beginHUD();
    colorMode(HSB,255);
    //colorMode(RGB, 255);
    infiniteCircle.drawVisual();
    cam.endHUD();
    break;
      
  case 2:
    colorMode(RGB,255);
    cam.beginHUD();
    rainVisual.drawVisual();
    cam.endHUD();
    break;
    
  case 3:
    colorMode(RGB,255);
    cam.beginHUD();
    infiniteTriangleVisual.drawVisual();
    cam.endHUD();
    break;
    
  case 4:
    cam.reset();
    colorMode(RGB,255);
    thirdVisual.drawVisual();
    break;
    
  case 5: 
    colorMode(RGB,255);
    cubeVisual.drawVisual();
    break;
    
  case 6:
    colorMode(HSB,255);
    cam.beginHUD();
    kaleidoscopeVisual.drawVisual();
    cam.endHUD();
    break;
    
  case 7:
    cam.beginHUD();
    //cam.setActive(false);
    colorMode(HSB,255);
    firstVisual.drawVisual();
    cam.endHUD();
    break;
    
  case 8:
    cam.beginHUD();
    //cam.setActive(false);
    colorMode(RGB,255);
    starFieldVisual.drawVisual();
    cam.endHUD();
    break;
    
  case 9:
    cam.beginHUD();
    //cam.setActive(false);
    colorMode(RGB,255);
    secondVisual.drawVisual();
    cam.endHUD();
    break;


    

  }
  
  ipTextTD = ipTextFieldTD.getText();
  ipTextJuce = ipTextFieldJuce.getText();
  
  //println(currentVisual + "    " + current_bpm);

  if(playButton.isMousePressed()&& playButtonClicked == false){
    
    createOSCConnection();
    
    playButtonClicked = true;
    background(0);
    print(playButtonClicked);
    playButton.hide();
    //currentVisual=(int)random(1,totVisuals);     //scelgo la prima visual in modo random
    currentVisual = 6;
   }
   
   
  //text(cp5.get(Textfield.class,"input").getText(), 360,130);
  
  
}

//scheduler creato per mandare messaggi OSC a touchdesigner
void createScheduleTimer(final float seconds){
  
  
  hasFinished = false;
  
  timer.schedule(new TimerTask(){
    public void run(){
      
      //println(" "+nf(seconds,1,2));
      hasFinished = true;
      
      OscMessage myMessage = new OscMessage("/toggle");
      myMessage.add(0); //mando 1 quando voglio staccare
      oscP5.send(myMessage, myRemoteLocation_sendToTouchDesigner); 
      
    }
  }, (long) (seconds*1e3));
   
    
  }
  

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  // print the address pattern and the typetag of the received OscMessage 
  /*print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
  println(" value"+theOscMessage.arguments());
  println("##VALUE "+theOscMessage.get(0).floatValue());
  */
  //println(theOscMessage.address());
  //println("/"+ipTextTD + "   " + theOscMessage.address());
  
  if(theOscMessage.address().equals("/"+ipTextTD)){
  if (theOscMessage.addrPattern().equals("/motion")){
    detection_motion = +theOscMessage.get(0).floatValue();
    mapMotion();
    smoothMotion(currentMotion);
    }
  }
  
  if(theOscMessage.address().equals("/"+ipTextJuce)){
  if (theOscMessage.addrPattern().equals("/juce/bpm")){
    //detection_bpm = +theOscMessage.get(0).intValue();
    
    int last_bpm = +theOscMessage.get(0).intValue();
    counterBPM++;   //aggiorno 
    println("NUOVO BPM ARRIVATO: " + last_bpm, "ID : ", +counterBPM);
    updateCurrentBPM(counterBPM, last_bpm); 
    //println(currentVisual);

  }
  if (theOscMessage.addrPattern().equals("/juce/velocity")){
    prev_velocity = detection_velocity;
    detection_velocity = +theOscMessage.get(0).floatValue();
    this.handleTransition((int)detection_velocity);
    mapVelocity();
  }
  if (theOscMessage.addrPattern().equals("/juce/beat")){
    detection_beat = +theOscMessage.get(0).floatValue();

    toggle_beat = !toggle_beat;

  }
  
  if (theOscMessage.addrPattern().equals("/juce/panning")){
    detection_panning = +theOscMessage.get(0).floatValue();
    smoothPanning();
  }
  
  if (theOscMessage.addrPattern().equals("/juce/centroid")){
    detection_centroid = +theOscMessage.get(0).floatValue();
    mapCentroid();
    
  }
  
  if (theOscMessage.addrPattern().equals("/juce/continuePanning")){
    prev_continuePanning = detection_continuePanning;
    detection_continuePanning = +theOscMessage.get(0).floatValue();
    //smoothContinuePanning();
    
    }
  }
  
    
  //println("BEAT: " +detection_beat);
  //println("MOVEMENT: " +detection_motion);
  //println("VELOCITY: " +detection_velocity);
    //println("BPM: " +detection_bpm);
  //println("PANNING: " +detection_panning);
  //println("CENTROID: " +detection_centroid);
  
} 

//localCounterBPM is the number of arrival of the detected bpm. 
//local_bpm is the detected bpm
void updateCurrentBPM(final int localCounterBPM, final int local_bpm){
  
    
    
    timer.schedule(new TimerTask(){
    public void run(){
      
      if (localCounterBPM == counterBPM){
          println("ACCEPTED BPM: ", +local_bpm, "ID : ", +localCounterBPM);
          detection_bpm = local_bpm;
          
          if(currentVisual!=0 && !manualMode){    //don't update if we are in the menu or we are in manual mode
            schedulerVisual();    //start the schedule if no other bpm are detected in 4 seconds
          }
      }
      
      else{
        println("NO UPDATE: another BPM value is detected in the choosen interval of time");
      }
    }
  }, (long) (timeToWaitOtherBPM*1e3));
}


//ASSOCIAZIONE BPM A VISUAL
void schedulerVisual(){
  
  current_bpm = detection_bpm;
  
  int previousVisual = currentVisual;
  float randValue = random(1);
  //println(randValue +"  " +previousVisual +"   " +currentVisual +"   " +(randValue<=0.5));
  if(current_bpm>rangeBPMMatrix[0][0] && current_bpm<rangeBPMMatrix[0][1] && previousVisual !=1 && previousVisual !=2){
    
    if(randValue<=0.5){
      currentVisual=1;
    }
    else{
      currentVisual=2;
    }
    //println(previousVisual +"   " +currentVisual);  
      
    sendOSCMessageColor();
  }
    
  else if(current_bpm>=rangeBPMMatrix[1][0] && current_bpm<rangeBPMMatrix[1][1] && previousVisual !=3 && previousVisual !=4){
    
    if(randValue<=0.5)
      currentVisual=3;
    else
      currentVisual=4;
      
       //println(previousVisual +"   " +currentVisual);
    
    sendOSCMessageColor();
  }
    
  else if(current_bpm>=rangeBPMMatrix[2][0] && current_bpm<rangeBPMMatrix[2][1] && previousVisual !=5 && previousVisual !=6){
    
    if(randValue<=0.5)
      currentVisual=5;
    else
      currentVisual=6;
     //println(previousVisual +"   " +currentVisual);

    
    sendOSCMessageColor();
  }
    
  else if(current_bpm >=rangeBPMMatrix[3][0] && current_bpm<rangeBPMMatrix[3][1] && previousVisual !=7 && previousVisual !=8){
    
    if(randValue<=0.5)
      currentVisual=7;
    else
      currentVisual=8;
     //println(previousVisual +"   " +currentVisual);

    
    sendOSCMessageColor();
  }

  else if(current_bpm >=rangeBPMMatrix[4][0] && current_bpm< rangeBPMMatrix[4][1] && previousVisual != 9){
    currentVisual=9;
    sendOSCMessageColor();
  }
/* ATTENZIONE GLITCH
    if(currentVisual==5){
       cam.setDistance(0);
       cubeVisual.d = false;
       cam.setMaximumDistance(500);
       cam.setMinimumDistance(40);  
     }

     */
     
     
}

//mappo i valori del centroide tra 0 e 1, vedendo qual è il massimo che arriva
void mapCentroid(){
  
  if(detection_centroid>maxCentroid)
    maxCentroid=detection_centroid;
    prev_centroid = currentCentroid;
  currentCentroid = map(detection_centroid, 0, maxCentroid, -1, 1);  //mappo i valori del centroide tra -1 e 1
}




void mapMotion(){
   //println(this.sumMotion);
  if(detection_motion>maxMotion)
      maxMotion=detection_motion;
      
  currentMotion = map(detection_motion, 0, maxMotion, 0, 1);  //mappo i valori del motion fra 0 e 1
  currentMotionPannel = map(detection_motion, 0, maxMotion, -0.05, 1)*0.1;
 
  if(currentVisual!=0) {   //faccio la somma solo se la visual corrent non è il menù
    
  if(this.sumMotion+currentMotionPannel>=0 && this.sumMotion+currentMotionPannel<=fullMotionValue){   //100 è il valore definito della dimensione della barra
    this.sumMotion = this.sumMotion + currentMotionPannel;
  }
    
    
  if ((this.sumMotion+currentMotionPannel>fullMotionValue)&& (fullBar==false) ){
    //sendOSCMessageLight();
    println("PRIMO IF");
    fullBar = true;
    currentBeat = toggle_beat;
    //this.sumMotion=0;
  }
  
    //println(currentBeat, toggle_beat);
  if((currentBeat==!toggle_beat) && (fullBar==true) && (this.sumMotion+currentMotionPannel<0 || this.sumMotion+currentMotionPannel>fullMotionValue)){           //per azzerare la somma
    this.sumMotion=0; 
    sendOSCMessageLight();
    fullBar = false;
    println("SECONDO");
    }
  }

  
}

//messaggio OSC da mandare per attivare la luce
void sendOSCMessageLight(){
  //println("DATO MANDATO OSC");
  OscMessage myMessage = new OscMessage("/toggle");
  //OscMessage myMessage2 = new OscMessage("/toggle2");
  //OscMessage myMessage = new OscMessage("/B");
  
  //prevVal = 1-prevVal;
  myMessage.add(1); //mando 1 quando parte l' OSC

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation_sendToTouchDesigner); 
  
  
  final float waitTime = 10;
  createScheduleTimer(waitTime);  
}


void sendOSCMessageColor(){
  //println("DATO MANDATO OSC");
  OscMessage redMessage = new OscMessage("/R");
  OscMessage greenMessage = new OscMessage("/G");
  OscMessage blueMessage = new OscMessage("/B");
  
  int redValue = round(random(0,1));
  int greenValue = round(random(0,1));
  int blueValue = round(random(0,1));
  
  //Controllo msg consecutivi uguali o RGB 0,0,0 da evitare
  if((pred == redValue && pgreen == greenValue && pblue == blueValue) || (redValue==0 && greenValue==0 && blueValue==0) ){
    do{
    redValue = round(random(0,1));
    greenValue = round(random(0,1));
    blueValue = round(random(0,1));
    }while((pred == redValue && pgreen == greenValue && pblue == blueValue) || (redValue==0 && greenValue==0 && blueValue==0) );
  }
  
  pred = redValue;
  pgreen = greenValue;
  pblue = blueValue;
  
  /*
  if(redValue==0 && greenValue==0 && blueValue==0)      //per non avere tutti 0, sennò la grafica su touch è scura
    greenValue=1;
  */
  
  redMessage.add(redValue);
  greenMessage.add(greenValue);
  blueMessage.add(blueValue);
  
  /* send the message */
  oscP5.send(redMessage, myRemoteLocation_sendToTouchDesigner); 
  oscP5.send(greenMessage, myRemoteLocation_sendToTouchDesigner);
  oscP5.send(blueMessage, myRemoteLocation_sendToTouchDesigner);
  
  

}


void keyPressed(){
  
  prevVisual = currentVisual;
  
  if(keyCode == ENTER && currentVisual!=0){   //se premo enter fa un cambio automatico di grafica, e non sono nel menù
    
    
    
    //currentVisual =  round(random(1, totVisuals)); 
    currentVisual = (currentVisual+1)%totVisuals+1;
    //currentVisual = 10;
    
    if(currentVisual==prevVisual)    //per evitare che sia generata la stessa subito dopo;
      currentVisual = (currentVisual + 1)%totVisuals;
    
    prevVisual = currentVisual;      //aggiornamento prevVisual
    
    //println(currentVisual);
  
    if(currentVisual == 0)  //perchè non voglio la visual del menù
      currentVisual++;
      
     /*
     if(currentVisual==5){
       cam.setDistance(0);
       cubeVisual.d = false;
       cam.setMaximumDistance(500);
       cam.setMinimumDistance(40);
       
     } 
     */
     
     if(currentVisual==6){
       background(0);
     }
       
  }
  
  else if(key == TAB && currentVisual!=0){    //premo esc se voglio tornare al menù di partenza

      currentVisual = 0;
      playButton.show();   //mostro il bottone
  }
  
  else if((key == 'm' || key=='M') && currentVisual!=0){    //premo esc se voglio tornare al menù di partenza

      manualMode=!manualMode;
  }
  

}


//ANCORA INUTILIZZATA
//funzione che gestisce un array di velocity per attivare o meno qualche transizione
void handleTransition(int velocity_detected){
  
  float sum=0, average=0;
  
  if(arrayVelocity.size()>=sizeArrayVelocity){
    
   arrayVelocity.remove(0); 
  }
  
  
  if(arrayVelocity.size()<sizeArrayVelocity){
    
   arrayVelocity.add(velocity_detected); 
  }
  

  
  for(int i=this.arrayVelocity.size()-1; i>=0; i--){
    sum +=arrayVelocity.get(i);

  }

  average = sum/sizeArrayVelocity;
  
  //if(average<20){
    //if(transitionFlag==false){
      //firstVisual.firstTime=0;
    //}
    //transitionFlag=true;
    
  //}
    
  //else
    //transitionFlag=false;
}


//funzione che disegna il pannello di riempimento
void drawPanel(){
  
  
  //noStroke();
  pushMatrix();
  pGraphic.beginDraw();
  if(currentVisual==6){
    pGraphic.background(0, 255);
  }
  else 
  pGraphic.background(0, 0);
  
  translate(0,0);
  colorMode(RGB);
  stroke(150);
  strokeWeight(1);
  
  noFill();
  rect(10, 10, 30, 100);   //primo rect di contenimento
  
  
  color colorProva = color(150);
  //stroke(255); 
  fill(colorProva, 200);
  rect(10, 10, 30, this.sumMotion);
  noStroke();
  
  if(manualMode){
    fill(150);
    String s = "M";
    text(s, 34, 140);
  }

  else{

    String s = "";
    text(s, 34, 140);
  }
  pGraphic.endDraw();
  popMatrix();
  
  image(pGraphic, 0, 0); 
  
}




void smoothMotion(float currentMotion){
  
  
  if(motionList.size()<sizeMotionValues)
    motionList.add(currentMotion);
    
  else{
    motionList.remove(0);
    motionList.add(currentMotion);
  }
  
  for(int i=0; i<motionList.size(); i++)
    averageMotion+=motionList.get(i);
    
  averageMotion = averageMotion/sizeMotionValues;
  
  //println("average:",averageMotion);
  
}

void smoothPanning() {
  
  if(panningList.size()<100)
    panningList.add(detection_panning);
    
  else{
    panningList.remove(0);
    panningList.add(detection_panning);
  }
  
  for(int i=0; i<panningList.size(); i++)
    averagePanning+=panningList.get(i);
  
  prev_averagePanning = averagePanning;
  averagePanning = averagePanning/100;
}


  void mapVelocity(){
    scaledVelocity = map(detection_velocity, 0, 127, -1, 1);
  }
  
  
void createOSCConnection(){
  oscP5 = new OscP5(this,12000);
  oscP5_carm = new OscP5(this, 12001);
  myRemoteLocation_receiveFromTouchDesigner = new NetAddress(ipTextTD,12000);           //indirizzo di chi manda motion da TouchDesigner
  myRemoteLocation_receiveFromJuce = new NetAddress(ipTextJuce,12001);    //indirizzo di chi manda audio features da Juce
  myRemoteLocation_sendToTouchDesigner= new NetAddress(ipTextTD,12002);
  
  //println(ipTextTD);
  //println(ipTextJuce);
}


void smoothContinuePanning() {
  
  prev_averageContinuePanning = averageContinuePanning;
  
  if(panningContinueList.size()<25)
    panningContinueList.add(detection_continuePanning);
    
  else{
    panningContinueList.remove(0);
    panningContinueList.add(detection_continuePanning);
  }
  
  for(int i=0; i<panningList.size(); i++)
    averageContinuePanning+=panningContinueList.get(i);
  
  
  averageContinuePanning = averageContinuePanning/25;
}
