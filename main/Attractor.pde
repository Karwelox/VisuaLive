class Attractor{

float x = 0.01;
float y = 0.01;
float z = 0.01;
float a = 3;
float b = 2.7;
float c = 1.7;
float d = 2;
float e = 9;
boolean flag = true;
boolean distance = true;
float dy=0;
float hu = 10;
int currentVisual = 0;
ArrayList<PVector> points = new ArrayList<PVector>();


float angle = 0;

Attractor() {
 
  
}

void drawVisual() {
  cam.setDistance(5000);
  cam.setRotations(-angle/2, angle, 20); // double pitch, double yaw, double roll
  background(0);
  float dt = 0.01;
  float dx = (a * (y - x))*dt;
  float dy = (x * (b - z) - y)*dt;
  float dz = (x * y - c * z)*dt;
  x = x + dx;
  y = y + dy;
  z = z + dz;

  points.add(new PVector(x, y, z));

  translate(0, 0, -80);
  //translate(width/2, height/2);
  strokeWeight(1);
  scale(2);
  //stroke(255);
  noFill();

  float hu = 0;
  beginShape();
  for (PVector v : points) {
    stroke(hu, 255, 255);
    vertex(v.x, v.y,v.z);
    
    //rumore
    /*
    PVector offset = PVector.random3D();
    offset.mult(0.1*scaledVelocity);
    v.add(offset);
    */
    
    hu += 0.1;
    if (hu > 255) {
      hu = 0;
    }
  }
  endShape();

  angle = angle+(averageMotion/10)+0.005;
  //println(x,y,z);
  
  
}






void setupLorentz(){
x = 0.01;
y = 0;
z = 0;

a = 10+(scaledVelocity*10);
b = 28+(averagePanning*10);
c = (8.0/3.0)+(currentCentroid*10);

points = new ArrayList<PVector>();
angle = 0;
cam.reset();
/*
cam.setMinimumDistance(100);
cam.setMaximumDistance(1000);
*/
  }

}
