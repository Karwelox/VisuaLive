class Circle{
  PVector pos;
  float asseMag, asseMin; //asse magg è quello orizzontale
  float col;
  
  Circle(PVector pos, float asseMag, float asseMin){
    this.asseMag= asseMag;
    this.asseMin= asseMin;
    this.pos= pos;
  }
  
  void drawCircle(){
    colorMode(HSB,255);
    //PVector newPos = new PVector(averagePanning*1000,0);
    PVector newPos = new PVector(0,0);
    //float col= map(asseMag,0,width,0,255);
    fill(col,200,255);
    //noFill();
    noStroke();
    //stroke(0);
    //strokeWeight(2);
    ellipse(newPos.x,newPos.y,asseMag+(averageMotion*150),asseMin+(averageMotion*200));
    
  }
  
}
