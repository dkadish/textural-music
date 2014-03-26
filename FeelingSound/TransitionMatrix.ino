char transitionMatrix[N_NOTES][N_NOTES] = {
  {23,0,3,3,6,3,1,7,1,2,0,1},
  {0,3,0,0,0,1,0,0,0,0,0,0},
  {3,0,12,1,1,3,0,5,0,2,0,3},
  {3,0,1,9,0,1,1,3,1,0,1,0},
  {6,0,1,0,14,2,0,5,0,1,0,2},
  {4,1,3,1,1,13,0,4,2,2,0,2},
  {1,0,0,1,0,0,4,0,0,1,0,0},
  {7,0,5,3,5,4,1,25,1,2,1,5},
  {2,0,0,1,0,2,0,1,7,0,0,1},
  {2,0,1,0,2,2,1,1,0,7,0,1},
  {0,0,0,1,0,0,0,1,0,0,5,0},
  {1,0,3,1,1,2,0,5,1,1,0,10}
};

int getNextNote( int element_i){
  //char* probabilities = (char *)malloc(87*sizeof(char));
  //probabilities = transitionMatrix[element_i];
  //Serial.print("Getting next note: ");
  //Serial.println(element_i);
  
  int e = 0;
  int topElement = 0;
  int thisElement = 0;
  
  /*int i=0;
  //thisElement = transitionMatrix[element_i][i]*random(10);
  Serial.println(transitionMatrix[25][0], DEC);
  
  return 25;
  */
  for( int i=0; i < N_NOTES; i++){
//    Serial.println(i);
    thisElement = transitionMatrix[element_i][i]*random(10);
    if( thisElement > topElement ){
      topElement = thisElement;
      e = i;
    }
  }
  
  return e;
}

