
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

