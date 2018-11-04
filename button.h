
// button control pin
const byte BUTTON_PIN = D1;
OneButton button(BUTTON_PIN, false);

// This function will be called when the button was pressed 1 time
/*void clickFn() {
  #if defined(debug) && debug == true
  Serial.println("Button click.");
  #endif
}*/
  
// This function will be called when the button was pressed 2 times in a short timeframe.
/*void doubleclickFn() {
  #if defined(debug) && debug == true
  Serial.println("Button doubleclick.");
  #endif
}*/

// This function will be called once, when the button is pressed for a long time.
void longPressStartFn() {
  #if defined(debug) && debug == true
  Serial.println("Button longPress start");
  #endif
  
  buttonTimer = millis();
}

// This function will be called once, when the button is released after beeing pressed for a long time.
void longPressStopFn() {
  #if defined(debug) && debug == true
  Serial.println("Button longPress stop");
  #endif

  if (millis() - buttonTimer > longPressTime) {
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

void buttonSetup(){
  
  pinMode(BUTTON_PIN, INPUT);
  //button.attachClick(clickFn);
  //button.attachDoubleClick(doubleclickFn);
  button.attachLongPressStart(longPressStartFn);
  //button.attachDuringLongPress(longPressFn);
  button.attachLongPressStop(longPressStopFn);
  
}



