
// light control pin
const byte LIGHT_PIN = D2;

void ledSetup(){ 
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
}

void lightOn(){  
  digitalWrite(LIGHT_PIN, HIGH);
}

void lightOff(){  
  digitalWrite(LIGHT_PIN, LOW);
}

void blinkNTimes(int times){
  #if defined(debug) && debug == true
  Serial.printf("\r\nBlinking light %d times\r\n", times);
  #endif
  while( times-- > 0 ){
    delay(200);
    digitalWrite(LIGHT_PIN, HIGH);
    delay(200);
    digitalWrite(LIGHT_PIN, LOW);
  }
}
