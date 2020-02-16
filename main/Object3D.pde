class Object3D{
  
  //legend: 1=cube, 2=piramid
  float angle=0;
  PVector[] points = new PVector[9];
  float detection = 110;
  boolean local_toggle;
  float num_figure;
  
  PVector[] projected; 
  
  float[][] projections = {
                           {1,0,0},
                           {0,1,0}
                          };  
  float centerX, centerY, offset_mult;
  
  Object3D(float centerX, float centerY, float offset_mult, float num_figure){
    
    this.centerX = centerX;
    this.centerY = centerY;
    this.offset_mult = offset_mult;
    this.num_figure=num_figure;
    
    points[0] = new PVector(-0.5, -0.5, -0.5);
    points[1] = new PVector(0.5, -0.5, -0.5);
    points[2] = new PVector(0.5, 0.5, -0.5);
    points[3] = new PVector(-0.5, 0.5, -0.5);
    points[4] = new PVector(-0.5, -0.5, 0.5);
    points[5] = new PVector(0.5, -0.5, 0.5);
    points[6] = new PVector(0.5, 0.5, 0.5);
    points[7] = new PVector(-0.5, 0.5, 0.5);
    
    points[8] = new PVector(0,-0.5,0);
    
    
  }
  
  
  void drawSquare3D(boolean toggle_back){
   
    //background(0);
  translate(this.centerX, this.centerY);
  
  this.local_toggle = toggle_back;
  
  //rotate(angle);
  stroke(255);
  strokeWeight(10);
  noFill();
  
  float[][] rotationZ = {
    { cos(angle), -sin(angle), 0},
    { sin(angle), cos(angle), 0},
    { 0, 0, 1}
  };

  float[][] rotationX = {
    { 1, 0, 0},
    { 0, cos(angle), -sin(angle)},
    { 0, sin(angle), cos(angle)}
  };

  float[][] rotationY = {
    { cos(angle), 0, sin(angle)},
    { 0, 1, 0},
    { -sin(angle), 0, cos(angle)}
  };
  
  projected = new PVector[9];
  
  int index = 0;
  for(PVector v: points) {
    
    PVector rotated = matmul(rotationY, v);
    rotated = matmul(rotationX, rotated);
    rotated = matmul(rotationZ, rotated);
    PVector projected2d = matmul(projections, rotated);
    //float lato=constrain(detection_velocity*2*offset_mult, 2, 20);
    projected2d.mult(detection_velocity*offset_mult + 0.1);
    //projected2d.mult(lato);
    projected[index] = projected2d;
    //point(projected2d.x, projected2d.y);
    index++;
  }
  
  
  
  for(PVector v : projected){
  if (toggle_back == true){
    stroke(255);
  }
  else{
    stroke(0);
  }
    //stroke(255);
    //strokeWeight(detection_motion+5);
    noFill();
    //point(v.x, v.y); //disegna i punti
    
  }
  
  if(this.num_figure==1)
    drawCube();
    
  if(this.num_figure==2)
    drawPiramid();
  
  
  //primo piano
  
  
   
  
  angle += currentMotion*0.1;
    
    
  }
  
  void drawCube(){
    
    for (int i = 0; i < 4; i++) {
    connect(i, (i+1) % 4, projected);
    connect(i+4, ((i+1) % 4)+4, projected);
    connect(i, i+4, projected);
  }
  }
  
  void drawPiramid(){
    
    connect(2,6, projected);
  connect(3,7, projected);
  connect(2,3, projected);
  connect(6,7, projected);
  
  connect(2,8, projected);
  connect(3,8, projected);
  connect(6,8, projected);
  connect(7,8, projected);
  }
  
    void connect(int i, int j, PVector[] points){
  PVector a = points[i];
  PVector b = points[j];
  
  if (this.local_toggle == true){
  stroke(255);
  }
  else{
    stroke(0);;
  }
  
  strokeWeight(5+currentMotion);
  //stroke(255);
  line(a.x, a.y,b.x,b.y);
  
  
} 
  
  

  
  
  
}
