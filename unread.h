
int getAlerts() {    // a function to get the number of unread emails in your Gmail inbox
  WiFiClientSecure client; // Use WiFiClientSecure class to create TLS (HTTPS) connection
  #if defined(debug) && debug == true
  Serial.printf("Connecting to %s:%d ... \r\n", host, httpsPort);
  #endif
  if (!client.connect(host, atoi(&httpsPort[0]))) {   // Connect to the Gmail server, on port 443
    #if defined(debug) && debug == true
    Serial.println("Connection failed");    // If the connection fails, stop and return
    #endif
    return -1;
  }

  #if defined(debug) && debug == true
  Serial.print("Check Certificate: ");
  Serial.println(fingerprint);
  #endif

  if (client.verify(fingerprint, host)) {   // Check the SHA-1 fingerprint of the SSL certificate
    #if defined(debug) && debug == true
    Serial.println("Certificate matches");
    #endif
  } else {                                  // if it doesn't match, it's not safe to continue
    #if defined(debug) && debug == true
    Serial.println("Certificate doesn't match");
    #endif
    return -1;
  }

  #if defined(debug) && debug == true
  Serial.print("Requesting URL: ");
  Serial.println(url);
  #endif

  String request = String("GET ") + url + " HTTP/1.1\r\n" + 
               "Host: " + host + "\r\n" +
               "Authorization: Basic " + credentials + "\r\n" +
               "User-Agent: "+ device +"\r\n" +
               "Connection: close\r\n\r\n";

  client.print(request);                          // Send the HTTPS request headers

  #if defined(debug) && debug == true
  Serial.println("Request sent: ");
  Serial.println(request);
  #endif

  unread = -1;
  alerts = -1;

  while (client.connected()) {                   // Wait for the response. The response is in XML format
    
    client.readStringUntil('<');                 // read until the first XML tag
    tagname = client.readStringUntil('>');       // read until the end of this tag to get the tag name
    #if defined(debug) && debug == true
    Serial.println(tagname);   
    #endif
    if (tagname == "fullcount") {                // if the tag is <fullcount>, 
                                                 // the next string will be the number of unread emails
      unreadStr = client.readStringUntil('<');   // read until the closing tag (</fullcount>)
      unread = unreadStr.toInt();                // convert from String to int
      
      //#if defined(debug) && debug == true
      //Serial.println(unread); 
      //#endif
      alerts = unread; 

      if(unread > 0){ 
        alerts = 0;
        cycle = unread;
        while(cycle > 0){
          client.readStringUntil('<');
          tagname = client.readStringUntil('>');
          if (tagname == "entry") {
            client.readStringUntil('<');
            tagname = client.readStringUntil('>');
            if (tagname == "title") {
              title = client.readStringUntil('<');  
              if(title.indexOf(alertString) > 0){
                  //#if defined(debug) && debug == true
                  //Serial.println(title);
                  //#endif
                  alerts++;
              }
            }
            cycle--;
          }
        }
      }
      
      break;                                    // stop reading
    }                                           // if the tag is not <fullcount>, 
                                                // repeat and read the next tag
    
  }
  #if defined(debug) && debug == true
  Serial.println("Connection closed");
  #endif

  return alerts;                                // Return the number of unread emails
}
