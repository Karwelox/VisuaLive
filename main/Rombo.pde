class Rombo{
  boolean colored;
  float altezza, shift; //asse magg è quello orizzontale
  
  Rombo(float alt, boolean colored, float shift){
    this.altezza= alt;
    this.colored= colored;
    this.shift=shift;
  }
  
  void drawRombo(){
    if(colored==true){
      fill(150+(150*currentCentroid), 200, 200); //triangolo superiore
      //stroke(0);
      //strokeWeight(2);
      //triangle(-altezza/4,0,altezza/4,0,0,-altezza);
      triangle(-altezza/3,shift,altezza/3,shift,0,-altezza+shift);
      fill(150+(150*currentCentroid), 200, 60);  //triangolo inferiore
      //stroke(0);
      //strokeWeight(2);
      //triangle(-altezza/4,0,altezza/4,0,0,altezza);
      triangle(-altezza/3,shift,altezza/3,shift,0,altezza+shift);
    }
    else{
      fill(0); //triangolo superiore
      //stroke(0);
      //strokeWeight(2);
      //triangle(-altezza/4,0,altezza/4,0,0,-altezza);
      triangle(-altezza/3,shift,altezza/3,shift,0,-altezza+shift);
    
      fill(0);  //triangolo inferiore
      //stroke(0);
      //strokeWeight(2);
      //triangle(-altezza/4,0,altezza/4,0,0,altezza);
      triangle(-altezza/3,shift,altezza/3,shift,0,altezza+shift); 
    }
    
    //noFill();
    //noStroke();
  }
  
  void shiftDown(float ALT){
    
    shift+=ALT/6000;
  }
  
  
}
