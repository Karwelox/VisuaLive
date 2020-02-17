class ParticleSystem{
  ArrayList<Particle> particles;
  PVector origin;
  
  PVector centerBallLocation; //posizione dell attrattore, da rendere modulabile
  float attractorMass = 100; 
  boolean stopDrawFlag = true;
  int hu;
  
  ParticleSystem(){
    this.particles = new ArrayList<Particle>();
    //float angle= random(0, 2*PI);
    this.origin = new PVector(width/2,height/2);
    centerBallLocation = new PVector(width/2, height/2);
  }
  ParticleSystem(PVector origin){
    this.particles = new ArrayList<Particle>();
    this.origin=origin.copy();
  }
  void addParticle(float centroidCenter){
    //println(centroidCenter);
    PVector actualCenter = new PVector((detection_continuePanning*width/2)+width/2, centroidCenter);
    //println((averagePanning*width/2)+width/2);
    this.particles.add(new Particle(actualCenter, 5, random(0,255)));
  }
  void action(float detection_motion){
    
    if(toggle_beat && stopDrawFlag==true){ //ad ogni beat attrezza il cambio colore
     hu = (int)random(255);
      stopDrawFlag=false;
  }  
 
  if(!toggle_beat)
    stopDrawFlag=true;
    
    Particle p;
    for(int i=this.particles.size()-1; i>=0; i--){
      p=this.particles.get(i);
      
      //if(!transitionFlag){
        p.applyForce(new PVector(random(-0.05, 0.05)*(detection_motion*10+1) , random(-0.05,0.05)*(detection_motion*10+1)));
        p.applyAttractionForce(centerBallLocation, detection_motion+ random(-0.005, 0.005)); //det = massa
      //}
     //else   
      //p.applyForce(new PVector(random(-0.05, 0.05), random(-0.05,0.05)));
      
      p.action(this.hu);
      
      
      
      p.lifespan-=0.1;
      if(p.isDead()){
        particles.remove(i);
        //this.addParticle();
      }
    }
  }
  
  void updateAttractorMass(){
    this.attractorMass = detection_velocity;
  }
  
}
