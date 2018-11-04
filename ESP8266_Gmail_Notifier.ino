/* ESP FileSystem management library  */
#include <FS.h> 

/* Wifi connections management libraries  */
#include <WiFiClientSecure.h>   // Include the HTTPS requests library
#include <ESP8266WiFi.h>        // Include the standard ESP Wi-Fi library
#include <WiFiManager.h>        // Include WiFiManager library 0.14.0v https://github.com/tzapu/WiFiManager 

/* Json management library */
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson

/* Button management library */
#include "OneButton.h" // https://github.com/mathertel/OneButton 1.3.0v

/* params file with default, constant values or function to retrieve them */
#include "params.h"

/* file system functions */ 
#include "fs.h"

/* wifi functions */ 
#include "wifi.h"

/* push button functions */ 
#include "button.h"

/* led functions */ 
#include "led.h"

/* alert unread email core feature function */ 
#include "unread.h"

void setup() {
  #if defined(debug) && debug == true
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  #endif

  ledSetup();

  buttonSetup();
  
  readDefaults();

  tempFailedTries = atoi(&failedTries[0]);
  
  checkTimer = millis();  //initial start check time
}

void loop() {
  
  button.tick();
  while (WiFi.status() != WL_CONNECTED) { 
    wifiConnect();
  }
  
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - checkTimer >= atoi(&checkInterval[0]))  //test whether the period has elapsed
  {
    
    count = getAlerts();
    if (count == 0) {
      #if defined(debug) && debug == true
      Serial.println("\r\nYou've got no alerts emails");
      #endif
      lightOff();
      tempFailedTries = atoi(&failedTries[0]);
    } else if (count > 0) {
      #if defined(debug) && debug == true
      Serial.printf("\r\nYou've got %d alerts\r\n", count);
      #endif
      lightOn();
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
    //delay(atoi(&checkInterval[0]));
    
    checkTimer = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}
