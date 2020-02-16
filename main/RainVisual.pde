class RainVisual{
Drop[] drops;
RainVisual(){
    
    drops = new Drop[2000];
    for(int i=0; i<drops.length;i++){
        drops[i] = new Drop();
        }
  }


void drawVisual(){
  
  background(backgroundRain);
  
  drawPanel();
  
  for(int i=0; i<drops.length;i++){
    drops[i].show();
    drops[i].fall();
  }
}
}
