//CONTROLLI:
//detection_bpm: range da  ...      , gestisce la grandezza dei singoli cerchi
//detection_velocity: 
//detection_motion: 
//detection_beat: 
//detection_panning:  
//detection_centroid:  

float divNum, actual;
boolean augm;

class InfiniteCircle{
boolean localFlag=false;
float  FR=60, asseA, asseB, maxMag, maxMin, valMag, valMin, vel;
int  max=0;
ArrayList<Circle> circles;

InfiniteCircle(){
  background(0);

  
  //size(1200,700);
  asseA=asseB=30;
  vel=40;
  PVector pos = new PVector(0,0);
  circles = new ArrayList<Circle>();
  Circle circle1 =new Circle(pos, asseA, asseB);    //il primo è il più grande
  Circle circle2 =new Circle(pos, asseA, asseB);
  Circle circle3 =new Circle(pos, asseA, asseB);
  Circle circle4 =new Circle(pos, asseA, asseB);  
  Circle circle5 =new Circle(pos, asseA, asseB);
  Circle circle6 =new Circle(pos, asseA, asseB);
  Circle circle7 =new Circle(pos, asseA, asseB);
  Circle circle8 =new Circle(pos, asseA, asseB);
  Circle circle9 =new Circle(pos, asseA, asseB);
  Circle circle10 =new Circle(pos, asseA, asseB);   
  Circle circle11 =new Circle(pos, asseA, asseB);
  Circle circle12 =new Circle(pos, asseA, asseB);
  Circle circle13 =new Circle(pos, asseA, asseB);
  Circle circle14 =new Circle(pos, asseA, asseB);
  Circle circle15 =new Circle(pos, asseA, asseB);
  /*
  Circle circle16 =new Circle(pos, asseA, asseB);   
  Circle circle17 =new Circle(pos, asseA, asseB);
  Circle circle18 =new Circle(pos, asseA, asseB);
  Circle circle19 =new Circle(pos, asseA, asseB);
  Circle circle20 =new Circle(pos, asseA, asseB);
  */
  
  circles.add(circle1);
  circles.add(circle2);
  circles.add(circle3);
  circles.add(circle4);
  circles.add(circle5);
  circles.add(circle6);
  circles.add(circle7);
  circles.add(circle8);
  circles.add(circle9);
  circles.add(circle10);
  circles.add(circle11);
  circles.add(circle12);
  circles.add(circle13);
  circles.add(circle14);
  circles.add(circle15);
  /*
  circles.add(circle16);
  circles.add(circle17);
  circles.add(circle18);
  circles.add(circle19);
  circles.add(circle20);
  */
  divNum= (float) 255 / (float) (circles.size()-1);
  
  for (int i=0;i<circles.size();i++){   //arcobaleno di grigi
    actual=circles.get(i).col=divNum*i;
  }
}
  


void drawVisual(){
  
  background(0);
  
  
  
  translate(width/2, height/2);
  
  maxMag=0;
  for (int i=0;i<circles.size();i++){  //serve a trovare l'indice del cerchio di dimensione maggiore
    
    if(circles.get(i).asseMag>maxMag){
      max=i;
      maxMag=circles.get(i).asseMag;
    };
  }
  
  
  
  
  for (int i=0;i<circles.size();i++){   //disegna i cerchi dal più grande al più piccolo
    int j= (max+i)%circles.size();
    circles.get(j).drawCircle();
  }
  
  
  circles.get(max).asseMag+=detection_bpm/4;
  circles.get(max).asseMin+=detection_bpm/4;
  
  for (int i=0;i<circles.size()-1;i++){  //serve a trovare cambiare grandezze cerchi
    
    int j= (max+i)%circles.size();
    valMag= circles.get(j).asseMag/2;
    valMin= circles.get(j).asseMin/2;
    j= (j+1)%circles.size();
    circles.get(j).asseMag=valMag;
    circles.get(j).asseMin=valMin;
    
  }

  
  if(circles.get(max).asseMag>=width*5 || (localFlag!=toggle_beat && circles.get(max).asseMag>=width*2.5)){  //quando il cerchio diventa grande
    if (actual+divNum>=255){
      augm=false;
      circles.get(max).col=actual-divNum;
      actual-=divNum;
    }
    else if(actual-divNum<0){
      augm=true;
      circles.get(max).col=actual+divNum;
      actual+=divNum;
    }
    else if(augm==false){
      circles.get(max).col=actual-divNum;
      actual-=divNum;
    }
    else if(augm==true){
      circles.get(max).col=actual+divNum;
      actual+=divNum;
    }
   
    circles.get(max).asseMag=valMag;
    circles.get(max).asseMin=valMin;
    
    if(localFlag!=toggle_beat)
      localFlag=toggle_beat;
    
  }
  
  translate(-width/2, -height/2);
  drawPanel();
  
}

}
