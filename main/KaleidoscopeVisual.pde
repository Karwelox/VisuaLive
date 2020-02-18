class KaleidoscopeVisual{
int symmetry = 8;
int angle = 360 / symmetry;
float strokeSize = 4; // use + and - keys to change instead of slider
float xoff;
boolean stopDrawFlag = true;

 KaleidoscopeVisual() {
}

void drawVisual() {
  
  drawPanel();
  
  if(toggle_beat && stopDrawFlag==true){
    symmetry = (2*(round(random(3,6))));
    angle = 360/ symmetry;
    background(0);
    stopDrawFlag=false;
  }  
 
  if(!toggle_beat)
    stopDrawFlag=true;
 
 
  
  
  translate(width / 2, height / 2);
  float pCentroid = map(prev_centroid, -1, 1, 0, width);
  float actualCentroid = map(currentCentroid, -1, 1, 0, width);
  //float pPanning = map(prev_averageContinuePanning, -1, 1, 0, width/2);
  //float actualPanning = map(averageContinuePanning, -1, 1, 0, width/2);
  float pVelocity = map(prev_velocity, 0, 127, 0, height/2);
  float actualVelocity = map(detection_velocity, 0, 127, 0, height/2);
  
  //println(prev_averageContinuePanning, averageContinuePanning);
  int mx = (int)(actualCentroid) - width / 2;
  int my = (int)(actualVelocity) - height / 2;
  int pmx = (int)(pCentroid) - width / 2;
  int pmy = (int)(pVelocity) - height / 2;
  
  
    int hu = int(map(sin(radians(xoff)), -1, 1, 0, 255));
    xoff += 1;
    colorMode(HSB,255);
    //println(60+averageMotion*500);
    stroke(hu,255, 60+averageMotion*500, 100);
    
    for (int i = 0; i < symmetry; i++) {
      rotate(radians(angle));
      
      // float d = dist(mx, my, pmx, pmy);
      // float sw = map(d, 0, 16, 16, 2);
      float sw = strokeSize;
      strokeWeight(sw);
      
      line(mx, my, pmx, pmy);
      
      pushMatrix();
      scale(1, -1);
      line(mx, my, pmx, pmy);
      popMatrix();
    }
  }
  
}
