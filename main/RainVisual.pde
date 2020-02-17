    PImage img;
class RainVisual{
Drop[] drops;
RainVisual(){
  //inserire FS.jpg per il fullscreen
    img = loadImage("tumblerino2FS.jpg");
    drops = new Drop[2000];
    for(int i=0; i<drops.length;i++){
        drops[i] = new Drop();
        }
  }


void drawVisual(){
  
  //background(backgroundRain);
  image(img,0,0);
  tint(255, 50);
  
  drawPanel();
  
  for(int i=0; i<drops.length;i++){
    drops[i].show();
    drops[i].fall();
  }
}
}
