//CONTROLLI:
//detection_bpm: range..   , gestisce la velocità di scorrimento delle montagne      
//detection_velocity: gestisce l' altezza delle montagne 
//detection_motion: gestisce la grandezza del cerchio/luna
//detection_beat: 
//detection_panning:  
//detection_centroid:  


class ThirdVisual{
int cols, rows;
int scl=20;
int w = 2100;
int h= 1700;
float flying = 0;
boolean toggle_back=true;

float[][] terrain;

ThirdVisual(){
  cols = w / scl;
  rows = h/ scl;
  //voglio storare i valori dei vertx dell'asse z
  terrain = new float[cols][rows];
}
  void drawVisual(){
     
        flying -= (float)current_bpm/1500+0.01;
  strokeWeight(1);
  //creo offset per x e y
  float yoff = flying;
  for (int y=0; y<rows; y++){
    float xoff = 0;
    for(int x=0; x< cols; x++){
      terrain[x][y] = map(noise(xoff,yoff), 0, 1, -100, 100);
      xoff += 0.2;
    }
    yoff += 0.2;
  }
  background(10,10, 50-(currentCentroid*30));
  //noFill();
  cam.beginHUD();
  drawPanel();  //disegno il pannello
  cam.endHUD();
  translate(0, height/4);
  
  fill(255,255,102,200);
  translate(0, -50, -1200);
  circle(0, -400, 500 + averageMotion*400);
  translate(0,50,1000);
  
  rotateX(PI/3);
  translate(-w/2,-h/2);
  strokeWeight(1);
  stroke(240,0,240);
  fill(0,255);
  for (int y=0; y< rows-1; y++){
    beginShape(TRIANGLE_STRIP);
    for (int x=0; x< cols; x++){
      vertex(x*scl, y*scl, terrain[x][y]*detection_velocity*0.01);
      vertex(x*scl, (y+1)*scl, terrain[x][y+1]*detection_velocity*0.01);
      //rect(x*scl, y*scl, scl, scl);
    }
    endShape();
  }    
  }
}
