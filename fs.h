
void readDefaults(){
  //read configuration from FS json
  #if defined(debug) && debug == true
  Serial.println("mounting FS...");
  #endif

  if (SPIFFS.begin()) {
    #if defined(debug) && debug == true
    Serial.println("mounted file system");
    #endif
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      #if defined(debug) && debug == true
      Serial.println("reading config file");
      #endif
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        #if defined(debug) && debug == true
        Serial.println("opened config file");
        #endif
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          #if defined(debug) && debug == true
          Serial.println("\nparsed json");
          #endif
          
          strcpy(credentials, json["credentials"]);
          strcpy(alertString, json["alertString"]);
          strcpy(checkInterval, json["checkInterval"]);
          strcpy(failedTries, json["failedTries"]);
          strcpy(fingerprint, json["fingerprint"]);
          strcpy(host, json["host"]);
          strcpy(url, json["url"]);
          strcpy(httpsPort, json["httpsPort"]);
          
        } else {
          #if defined(debug) && debug == true
          Serial.println("failed to load json config");
          #endif
        }
      }
    }
  } else {
    #if defined(debug) && debug == true
    Serial.println("failed to mount FS");
    #endif
  }
  //end read
}

void formatFS(){  
  #if defined(debug) && debug == true
  Serial.println("Formatting FS");
  #endif
  //clean FS, for testing
  SPIFFS.format();
}
