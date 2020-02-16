//Visual che consiste in un nucleo o più nuclei che si ingrandiscono a seconda della velocity ricevuta in input
//e un particle system, in cui ogni particella aumenta la velocità in base alla velocity

//CONTROLLI:
//detection_bpm: range da  ...
//detection_velocity: modifica la grandezza dei cerchi
//detection_motion: modifica le forze che fanno muovere le singole particelle e il colore delle particles
//detection_beat: //  
//detection_panning:  modifica il centro su cui vengono disegnati i cerchi
//detection_centroid:  modifica il centro in cui viene disegnato il particle system


class FirstVisual {
  ParticleSystem ps;
   //massa dell attrattore, da rendere modulabile
  final Timer firstVisualTimer = new Timer();
  int states=2;
  int currentState=0; 
  boolean firstHasFinished = true;
  FirstVisual() {
    ps = new ParticleSystem();
    
  }

  void drawVisual() {
      

    background(0);
    drawPanel();  //disegno il pannello
    
    //se finiscono i secondi messi di default, cambia il modo in cui disegna il cerchio (in base allo stato) 0 sinistra, 1 entrambi, 2 destra
    if (this.firstHasFinished) {
    final float waitTime = random(5,20);
    createScheduleTimer(waitTime);
    this.currentState = (this.currentState+1)%this.states;
    //println("\n\nTimer scheduled for "+ nf(waitTime, 0, 2) + " secs.\n" + "      " + currentState); 
    }

    
    float actualcenterX = width/2 + detection_panning*100;   //centro che mappa il primo cerchio 
    float provaCenter = width/2 - detection_panning*100;     //centro che mappa il secondo cerchio
    
    float centroidCenter = height/2 - (height/2*currentCentroid);   //centro che mappa il punto in cui sono generate le particelle
    //println(detection_centroid);
    
    ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);
    /*ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);
    ps.addParticle(centroidCenter);*/
    
    ps.updateAttractorMass();
      
    rectMode(CORNER);  
    fill(0, 30);
    noStroke();
    rect(0, 0, width, height);

    float angle= random(0, 2*PI);
    
    
    

    ps.origin = new PVector(detection_velocity/2*sin(angle)+width/2, detection_velocity/2*cos(angle)+height/2);
    ps.action(currentMotion);

    fill(0, 5);   //da qui gestisco la trasparenza dei cerchi
    strokeWeight(2);
    stroke(255);
    
    //centro_x è mappato con il panning, centro_x con il panning, grandezza con la velocity
    switch(currentState){
      case 0:
        circle(actualcenterX, height/2, detection_velocity*4);
        
        break;
        
      case 1:
        circle(actualcenterX+100, height/2, detection_velocity*4);
        circle(provaCenter-100, height/2, detection_velocity*4);
        break;
        
      }
    
     
    
    
    }
    
    
    void createScheduleTimer(final float seconds){
  
  this.firstHasFinished = false;
  
  firstVisualTimer.schedule(new TimerTask(){
    public void run(){
      //print(" "+nf(seconds,1,2));
      firstHasFinished = true;
    }
  }, (long) (seconds*1e3));
   
    
  }
  
  
  
  
  
}
