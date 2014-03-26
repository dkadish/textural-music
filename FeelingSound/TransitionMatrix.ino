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
  
  int e = 0; // The position of the element to play
  int topElement = 0; // The score of the top-ranking element
  int thisElement = 0; // The current element's score
  
  for( int i=0; i < N_NOTES; i++){
    thisElement = transitionMatrix[element_i][i]*random(10); // Find this element's score
    
    // If it scores higher than the top element, set it as the top element
    if( thisElement > topElement ){
      topElement = thisElement;
      e = i;
    }
  }
  
  return e;
}

