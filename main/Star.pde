class Star {

  float x;
  float y;
  // I create "z", a variable I'll use in a formula to modify the stars position.
  float z;
  
  // I create an other variable to store the previous value of the z variable.
  float pz;

  Star() {
    x = random(-width/2, width/2);
    y = random(-height/2, height/2);
    z = random(width/2);
    // I set the previous position of "z" in the same position of "z",
    // which it's like to say that the stars are not moving during the first frame.
    pz = z;
  }

  void update(float speed) {
    
    //z parte alto. quindi parte lontano nello spazio 3D. più diminuisce e più si "avvicina" verso di noi. 
    //aggiorno continuamente z, e anche i valori sx e sy, dividendo la x e la y della stella per il valore attuale di z.
    //poichè tale valore diminuisce, tale divisione aumenta e quindi il valore delle nuove coordinate aumenta
    //dando l'impressione di avvicinarsi verso di noi

    z = z - speed;
    x = x + (detection_panning);
   //riaggiorno il valore della posizione quando z è <1, ovvero quando la stella è sicuramente fuori dai bordi.
   
    if (z < 1) {
      z = width/2;
      x = random(-width/2, width/2);
      y = random(-height/2, height/2);
      pz = z;
    }
  }

  void show() {
    fill(255);
    noStroke();

    // with theese "map", I get the new star positions
    // the division x / z get a number between 0 and a very high number,
    // we map this number (proportionally to a range of 0 - 1), inside a range of 0 - width/2.
    // In this way we are sure the new coordinates "sx" and "sy" move faster at each frame
    // and which they finish their travel outside of the canvas (they finish when "z" is less than a).

    float sx = map(x / z, 0, 1, 0, width/2);
    float sy = map(y / z, 0, 1, 0, height/2);;

    // I use the z value to increase the star size between a range from 0 to 16.
    float r = map(z, 0, width/2, 16, 0);
    //ellipse(sx, sy, r, r);

    // Here i use the "pz" valute to get the previous position of the stars,
    // so I can draw a line from the previous position to the new (current) one.
    float px = map(x / pz, 0, 1, 0, width/2);
    float py = map(y / pz, 0, 1, 0, height/2);

    // Placing here this line of code, I'm sure the "pz" value are updated after the
    // coordinates are already calculated; in this way the "pz" value is always equals
    // to the "z" value of the previous frame.
    pz = z;
  
    float sWeight = map (z, 0, width/2, 2, 0); //IL valore massimo (2) E' DA MAPPARE CON LA VELOCITY
    strokeWeight(sWeight);
    stroke(255);
    
    line(px, py, sx, sy);

  }
}
