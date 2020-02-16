//CONTROLLI:
//detection_bpm: range da  ...      , modifica la velocità della camera
//detection_velocity: 
//detection_motion: modifica l' angolo di rotazione del cubo
//detection_beat: modifica la distanza tra i singoli cubi 
//detection_panning:  
//detection_centroid: 



class CubeVisual{
  boolean d=false;
  float angle= 0;
  float counterLerpColor = 0;
  boolean changeSignCounter=false;
  color transitionColor=0;
  int distCubes = 10;

  CubeVisual(){
  
  
  }


void drawVisual() {
  
  handleBackground();
  background(this.transitionColor);
  cam.beginHUD();
  drawPanel();  //disegno il pannello
  stroke(0,0,0);
  cam.endHUD();
  randomSeed(10);
  //smooth(8);
  //rotazioni lungo assi x y z
  cam.setRotations(this.angle, this.angle, this.angle); // double pitch, double yaw, double roll
  //toggle zoom
  //cam.setDistance(cam.getDistance() + detection_velocity);
  
  if(cam.getDistance() > 400 && d==false)
   d = true;
   else if (cam.getDistance()< 51 && d ==true)
     d= false;
   //zoom out
  if(d == false){
    cam.setDistance(cam.getDistance()+(detection_bpm));
  }
  //zoom in
    else if(d == true){
     cam.setDistance(cam.getDistance()-(detection_bpm));
    }
    
    
  //println(cam.getDistance());
  
   if(toggle_beat){
     distCubes=0;
   }
   
   else{
          distCubes=7;      }
    
    //matrice cubi
  for (int x= -50-(distCubes*5); x<= 50+(distCubes*5); x+= 10+distCubes){
    for (int y= -50-(distCubes*5); y<= 50+(distCubes*10); y+= 10+distCubes){
      for (int z= -50-(distCubes*5); z<= 50+(distCubes*10); z+= 10+distCubes){
        pushMatrix();
        translate(x,y,z);
        boxTile();
        popMatrix();
      }
    }
  }
    angle=angle+averageMotion/100;

}
//cubi singoli
void boxTile(){
  fill(random(255), random(255), random(255));
  box(random(9));
}


void handleBackground(){
  this.transitionColor= lerpColor(0, 50, counterLerpColor);
  
  
  if(counterLerpColor>1)
      changeSignCounter=true;
      
  if(counterLerpColor<0)
      changeSignCounter=false;
      
   if(changeSignCounter)
     counterLerpColor-=0.001;
     
     
   if(!changeSignCounter)
     counterLerpColor+=0.001;

     
   
  
 
}


     
  
  
}




  
