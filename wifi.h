
void WifiReset(){
  
  #if defined(debug) && debug == true
  Serial.println("Resetting WiFi saved params");
  #endif
  
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
  wifiManager.resetSettings();
  
}

void wifiConnect() {
  #if defined(debug) && debug == true
  Serial.println("Not Connected to WIFI try to connect, otherwise create esp own wifi prefix-chipID");
  #endif
  
  WiFiManagerParameter custom_credentials("credentials", "Email credentials (see docs)", credentials, 45);
  WiFiManagerParameter custom_alertString("alertString", "Subject alert string", alertString, 20);
  WiFiManagerParameter custom_checkInterval("checkInterval", "Check every XXX milliseconds", checkInterval, 6);
  WiFiManagerParameter custom_failedTries("failedTries", "How many failed try before alerts", failedTries, 2);
  WiFiManagerParameter custom_fingerprint("fingerprint", "SHA-1 fingerprint SSL of Gmail server", fingerprint, 60);
  WiFiManagerParameter custom_host("host", "Gmail server", host, 40);
  WiFiManagerParameter custom_url("url", "Gmail path", url, 60);
  WiFiManagerParameter custom_httpsPort("httpsPort", "Gmail server port", httpsPort, 6);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //add all your parameters here
  wifiManager.addParameter(&custom_credentials);
  wifiManager.addParameter(&custom_alertString);
  wifiManager.addParameter(&custom_checkInterval);
  wifiManager.addParameter(&custom_failedTries);
  wifiManager.addParameter(&custom_fingerprint);
  wifiManager.addParameter(&custom_host);
  wifiManager.addParameter(&custom_url);
  wifiManager.addParameter(&custom_httpsPort);
  
  //sets timeout before webserver loop ends and exits even if there has been no setup.
  //useful for devices that failed to connect at some point and got stuck in a webserver loop
  wifiManager.setConfigPortalTimeout(120); 
  
  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect(device.c_str());
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  #if defined(debug) && debug == true
  // if you get here you have connected to the WiFi
  Serial.println("Connected. IP: ");
  Serial.println(WiFi.localIP());
  #endif
  
  strcpy(credentials, custom_credentials.getValue());
  strcpy(alertString, custom_alertString.getValue());
  strcpy(checkInterval, custom_checkInterval.getValue());
  strcpy(failedTries, custom_failedTries.getValue());
  strcpy(fingerprint, custom_fingerprint.getValue());
  strcpy(host, custom_host.getValue());
  strcpy(url, custom_url.getValue());
  strcpy(httpsPort, custom_httpsPort.getValue());
  
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    
    //json["output"] = output;
    json["credentials"] = credentials;
    json["alertString"] = alertString;
    json["checkInterval"] = checkInterval;
    json["failedTries"] = failedTries;
    json["fingerprint"] = fingerprint;
    json["host"] = host;
    json["url"] = url;
    json["httpsPort"] = httpsPort;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }
}

