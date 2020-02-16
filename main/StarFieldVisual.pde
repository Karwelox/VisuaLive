class StarFieldVisual{
Star[] stars = new Star[1000];

// I create a variable "speed", it'll be useful to control the speed of stars.
float speed;
StarFieldVisual(){
  
  for (int i = 0; i < stars.length; i++) {
    stars[i] = new Star();
  }
  
}


void drawVisual() {
  
  speed = map(currentMotion+0.05, 0, 1, 0, 30); //valore della velocità preso dal currentMotion
  background(0);
  
  drawPanel();

  translate(width/2, height/2);

  for (int i = 0; i < stars.length; i++) {
    stars[i].update(speed);
    stars[i].show();
  }
}
}
