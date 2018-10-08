/* ESP FileSystem management library  */
#include <FS.h> 

/* Wifi connections management libraries  */
#include <WiFiClientSecure.h>   // Include the HTTPS requests library
#include <ESP8266WiFi.h>        // Include the standard ESP Wi-Fi library
#include <WiFiManager.h>        // Include WiFiManager library 0.14.0v https://github.com/tzapu/WiFiManager 

/* Json management library */
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

/* params file with default, constant values or function to retrieve them */
#include "params.h"

/* useful functions */ 
#include "utils.h"

/* file system functions */ 
#include "fs.h"

/* wifi functions */ 
#include "wifi.h"

/* push button functions */ 
#include "button.h"

/* alert unread email core feature function */ 
#include "unread.h"

void setup() {
  #if defined(debug) && debug == true
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  #endif
  
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
  
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(BUTTON_PIN, resetButtonOn, RISING);
  
  //formatFS();
  //WifiReset();
  
  readDefaults();

  tempFailedTries = atoi(&failedTries[0]);
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) { 
    wifiConnect();
  }
  count = getAlerts();
  if (count == 0) {
    #if defined(debug) && debug == true
    Serial.println("\r\nYou've got no alerts emails");
    #endif
    digitalWrite(LIGHT_PIN, LOW);
    tempFailedTries = atoi(&failedTries[0]);
  } else if (count > 0) {
    #if defined(debug) && debug == true
    Serial.printf("\r\nYou've got %d alerts\r\n", count);
    #endif
    digitalWrite(LIGHT_PIN, HIGH);
    tempFailedTries = atoi(&failedTries[0]);
  } else {
    #if defined(debug) && debug == true
    Serial.println("Could not get alerts mails");
    #endif
    if( tempFailedTries-- <= 0){
      blinkNTimes(5);
      tempFailedTries = -1;
    }
  }
  #if defined(debug) && debug == true
  Serial.println('\n');
  #endif
  delay(atoi(&checkInterval[0]));
}

