class InfiniteTriangleVisual{
  
float altezza=30, maxAlt, valAlt, vel=40, shift, ALT;
int  max=0;
ArrayList<Rombo> rombi;
boolean colorato=true, nero=false;

InfiniteTriangleVisual(){

  //colorMode(HSB,255);
  //size(1200,700);
  rombi = new ArrayList<Rombo>();
  Rombo rombo1 =new Rombo(altezza, colorato, 0);  //true sarà bianco-colorato
  Rombo rombo2 =new Rombo(altezza, nero, 0);  //false sarà nero
  Rombo rombo3 =new Rombo(altezza, colorato, 0);
  Rombo rombo4 =new Rombo(altezza, nero, 0);
  Rombo rombo5 =new Rombo(altezza, colorato, 0);
  Rombo rombo6 =new Rombo(altezza, nero, 0);
  Rombo rombo7 =new Rombo(altezza, colorato, 0);
  Rombo rombo8 =new Rombo(altezza, nero, 0);
  Rombo rombo9 =new Rombo(altezza, colorato, 0);
  Rombo rombo10 =new Rombo(altezza, nero, 0);
  Rombo rombo11 =new Rombo(altezza, colorato, 0);
  Rombo rombo12 =new Rombo(altezza, nero, 0);
  Rombo rombo13 =new Rombo(altezza, colorato, 0);
  Rombo rombo14 =new Rombo(altezza, nero, 0);
  
  rombi.add(rombo1);
  rombi.add(rombo2);
  rombi.add(rombo3);
  rombi.add(rombo4);
  rombi.add(rombo5);
  rombi.add(rombo6);
  rombi.add(rombo7);
  rombi.add(rombo8);
  rombi.add(rombo9);
  rombi.add(rombo10);
  rombi.add(rombo11);
  rombi.add(rombo12);
  rombi.add(rombo13);
  rombi.add(rombo14);
  
}

void drawVisual(){
  
  background(0);
  
  
  
  translate(width/2, height*2/3);
  noStroke();
  this.maxAlt=0;
  for (int i=0;i<rombi.size();i++){  //serve a trovare l'indice del maggiore
    
    if(rombi.get(i).altezza>maxAlt){
      this.max=i;
      this.maxAlt=rombi.get(i).altezza;
    };
  }
  
  
  for (int i=0;i<rombi.size();i++){   //disegna cerchi
    int j= (max+i)%rombi.size();
    rombi.get(j).drawRombo();
  }
  
  for (int i=0;i<rombi.size();i++){   //shifta in giù i rombi
    
    if(rombi.get(i).colored==true){
      rombi.get(i).shiftDown(rombi.get(i).altezza);
      ALT=rombi.get(i).altezza;
    }
    else
      rombi.get(i).shiftDown(ALT);
  }
  
  //rombi.get(max).altezza+=vel;
  rombi.get(max).altezza+=(detection_bpm)*((averageMotion+0.01)*10);
  
  for (int i=0;i<rombi.size()-1;i++){  //serve a trovare cambiare grandezze cerchi
    
    int j= (max+i)%rombi.size();
    valAlt= rombi.get(j).altezza/2;
    
    j= (j+1)%rombi.size();
    rombi.get(j).altezza=valAlt;
    
    
  }

  
  if(rombi.get(max).altezza>width*5){   //se diventa troppo grande lo si fa diventare il più piccolo
    rombi.get(max).shift=0;
    rombi.get(max).altezza=1;
    //rombi.get(max).altezza=valAlt;     //valMag contiene il valore dell'oggetto più piccolo
  }
  
  translate(-width/2, -height*2/3);
  drawPanel();
  
}
}
