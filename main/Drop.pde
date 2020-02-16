class Drop{
  
  
  float x = random(-100, width+100);
  float y = random(-900,-50);
  float z = random(0,20);
  float len = map(z, 0, 20, 10, 20);    //più è lunga, più è vicina
  float yspeed= map(z, 0, 20, 1, 20);   //robe più vicine sono più veloci
  
  void fall(){
    
    //y = y+yspeed;
    
    //float grav = map(z, 0, 20, 0, 0.2);
    //yspeed = yspeed + grav; 
   
    
     y = y+(yspeed*((averageMotion)+0.2));
     
     
    
    if(y>height)
      y = random(-200,-100);
      yspeed= map(z, 0, 20, 4, 10); 

  }
  
  void show(){
    float thick = map(z, 0, 20, 0.5, 3);   //stroke 0.5 quando è lontano, 3 quando è vicino
    strokeWeight(thick);
    //float mapY = map(y, 0, height, 50, 255);
    //stroke(mapY);   //da mappare con la velocity
    stroke((int)detection_velocity*2,50,250, 140+(scaledVelocity*80));

    line(x,y,x,y+len);
    
  }
  
  
}
