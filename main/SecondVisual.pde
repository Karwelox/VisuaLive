//CONTROLLI:
//detection_bpm: range da  ...      
//detection_velocity: gestisce la grandezza degli oggetti 3D
//detection_motion: gestisce lo spessore delle line degli oggetti, e la loro velocità di rotazione 
//detection_beat: fa il toggle black-white dello sfondo e della figura
//detection_panning: 
//detection_centroid:  




class SecondVisual{
 
  Object3D cube;
  Object3D piramid;

  float offset_mult;
  
    
  SecondVisual(){

    cube = new Object3D(width/2, height/2,5.4, 1);
    piramid = new Object3D(width/2, height/2, 5.4, 2);

    
    
  }
  
  
    

  
  
  void drawVisual(){
    
  
    
      if (toggle_beat == true){
        background(0);
        pushMatrix();
        cube.drawSquare3D(toggle_beat);
        popMatrix();
    
  }
  else{
     background(255);
     pushMatrix();
    piramid.drawSquare3D(toggle_beat); 
    popMatrix();
    }
    
  
  pushMatrix();
  drawPanel();  //disegno il pannello
  popMatrix();
  
} 


}
