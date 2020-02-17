class MainMenu{
  PFont f;
  PFont ipFont;
  String target;
  int popmax;
  float mutationRate;
  Population population;
  
  
  MainMenu(String target){
      f = createFont("Courier", 32, true);
      ipFont = createFont("Courier", 15, true);
    this.target = target;
    popmax = 350;
    mutationRate = 0.005;
    
  population = new Population(target, mutationRate, popmax);
  
  
  }



void drawText() {
  
  colorMode(RGB,255);
  
  // Generate mating pool
  population.naturalSelection();
  //Create next generation
  population.generate();
  // Calculate fitness
  population.calcFitness();
  //displayInfo();
  String answer = population.getBest();
  textFont(f);
  textAlign(CENTER);
  background(backgroundImage);
  fill(255);
  textSize(100);
  text("VisuaLive",width/2,height/3);
  textSize(40);
  text(answer, width/2, height/2);
  textAlign(RIGHT);
  textSize(30);
  text("Edoardo Epifani", width-10,height-90);
  text("Carmelo Fascella", width-10,height-60);
  text("Francesco Pino", width-10,height-30);
  
  playButtonClicked = false;

  
  
}

void displayInfo() {
  background(255);
  // Display current status of populationation
  String answer = population.getBest();
  textFont(f);
  textAlign(LEFT);
  fill(0);
  
  
  textSize(16);
  text("Best phrase:",20,30);
  textSize(32);
  text(answer, 20, 75);

  textSize(12);
  text("total generations: " + population.getGenerations(), 20, 140);
  text("average fitness: " + nf(population.getAverageFitness(), 0, 2), 20, 155);
  text("total populationation: " + popmax, 20, 170);
  text("mutation rate: " + int(mutationRate * 100) + "%", 20, 185);
 
  textSize(10);
  text("All phrases:\n" + population.allPhrases(), 650, 10);
}

void drawBlack(){
  background(0);
  
}

void drawPlayButton(){
    stroke(255);
    noFill();
    strokeWeight(2);
    textFont(f);
    rect(width/2-49,599,100,32);
    //String str = "PLAY";
    //text(str, width/2+38, 624);  
}

}
