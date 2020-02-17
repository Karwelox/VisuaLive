class Particle{
 PVector location, velocity, acceleration;
 float radius_circle, lifespan, mass=1;

 Particle(PVector location, float radius_circle, float lifespan){
   this.location = location.copy();
   this.velocity = new PVector();
   this.acceleration = new PVector();
   this.radius_circle=radius_circle;
   this.lifespan=lifespan;
   this.mass = 1;
}
  //update della posizione
  void planning(){
    this.velocity.add(this.acceleration);
    this.location.add(this.velocity);
    this.acceleration.mult(0); //multiply a value
  }
  
  //qui viene imposta l'accelerazione
  void applyForce(PVector force){
    this.acceleration.add(force);
  }
  
  void action(int hu){
    colorMode(HSB,255);
    this.planning();
    fill(hu, 255,40+(averageMotion*600), this.lifespan);
    noStroke();
    ellipse(this.location.x, this.location.y, this.radius_circle, this.radius_circle);
  }
  
  boolean isDead(){
    return this.lifespan<=0;
  }
  
  
   void applyAttractionForce(PVector centerBallLocation, float attractorMass){
    PVector attr_force=centerBallLocation.copy();
    attr_force.sub(this.location);
    float dist=attr_force.mag();
    dist=constrain(dist, 50,100);
    attr_force.normalize(); 
    attr_force.mult(attractorMass*this.mass/(dist*dist));
    this.applyForce(attr_force);       
    // if(drag_force.x!=drag_force.x || drag_force.y!=drag_force.y){exit();}     
  }

}
