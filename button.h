
void resetButtonOn(); 

/* RESET BUTTON PRESSED UP */
void resetButtonOnFinal() {
  detachInterrupt(BUTTON_PIN);
  attachInterrupt(BUTTON_PIN, resetButtonOn, RISING);

  #if defined(debug) && debug == true
  Serial.println(currentMillis);
  #endif
  
  if (currentMillis + intervalFlashEeprom < millis()) {
    #if defined(debug) && debug == true
    Serial.println("Reset Eeprom");
    #endif

    delay(100);
    // do action
    formatFS();
    WifiReset();

    #if defined(debug) && debug == true
    Serial.flush();
    #endif
  }
}
/* RESET BUTTON PRESSED UP */

/* RESET BUTTON PRESSED DOWN */
void resetButtonOn() {
  #if defined(debug) && debug == true
  Serial.println("");
  Serial.print("Hold RESET Button 7 seconds for reset the LAMP, ");
  Serial.println(currentMillis);
  #endif
  
  detachInterrupt(BUTTON_PIN);
  attachInterrupt(BUTTON_PIN, resetButtonOnFinal, FALLING);
  currentMillis = millis();
}
/* RESET BUTTON PRESSED DOWN */
