
#define LEAF_1 5 //white
#define LEAF_2 6 // purple
#define LIGHT 7
#define LIGHT_CHANCE 12
#define LEAF_1_CHANCE 6
#define LEAF_2_CHANCE 3

boolean light_on;
boolean leaf_1_on;
boolean leaf_2_on;
long r;

void setup(){
  light_on = true;
  leaf_1_on = true;
  leaf_2_on = true;
  
  pinMode(LIGHT, OUTPUT);
  pinMode(LEAF_1, OUTPUT);
  pinMode(LEAF_2, OUTPUT);
  
  digitalWrite(LIGHT, HIGH);
  digitalWrite(LEAF_1, HIGH);
  digitalWrite(LEAF_2, HIGH);
}

void loop(){
  if( light_on ){
    
    if( leaf_1_on ){
      //leaf_1_on = randomOnOff( LEAF_1, LEAF_1_CHANCE, false);
      digitalWrite(LEAF_1, LOW);
      leaf_1_on = !leaf_1_on;
    } else {
      //leaf_1_on = randomOnOff( LEAF_1, LEAF_1_CHANCE/2, true);
      digitalWrite(LEAF_1, HIGH);
      leaf_1_on = !leaf_1_on;
    }
    
    if( leaf_2_on ){
      //leaf_2_on = randomOnOff( LEAF_2, LEAF_2_CHANCE, false);
      digitalWrite(LEAF_2, LOW);
      leaf_2_on = !leaf_2_on;
    } else {
      //leaf_2_on = randomOnOff( LEAF_2, LEAF_2_CHANCE/2, true);
      digitalWrite(LEAF_2, HIGH);
      leaf_2_on = !leaf_2_on;
    }
    
    if( randomOnOff( LIGHT, LIGHT_CHANCE, false) ){
      light_on = false;
      digitalWrite(LEAF_1, LOW);
      digitalWrite(LEAF_2, LOW);
      
      leaf_1_on = false;
      leaf_2_on = false;
    }
  } else {
    if( randomOnOff(LIGHT, LIGHT_CHANCE/4, true) ){
      light_on = true;
      digitalWrite(LEAF_1, LOW);
      digitalWrite(LEAF_2, HIGH);
      
      leaf_1_on = true;
      leaf_2_on = true;
    }
  }
  
  delay(5*1000);
}

boolean randomOnOff(int pin, long chance, boolean high){
    r = random(0,chance);
    
    if( r == 0 ){
      digitalWrite(pin, high ? HIGH : LOW );
      
      return( true );
    }
    
    return false;
}
