/* FORKED FROM https://tttapa.github.io/ESP8266/Chap17%20-%20Email%20Notifier.html */

#include <WiFiClientSecure.h>   // Include the HTTPS library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

const char* host = "mail.google.com"; // the Gmail server
const char* url = "/mail/feed/atom";  // the Gmail feed url
const int httpsPort = 443;            // the port to connect to the email server
// Go to https://www.google.com/settings/security/lesssecureapps to enable access for "less secure apps":

                                      // The SHA-1 fingerprint of the SSL certificate for the Gmail server (see below)
const char* fingerprint = "F9 1E 8F 0B 1A 8A 79 0F 6F 10 29 17 BD A5 1C 63 54 73 23 C6";
// following the shell command for filling up the above variable: 
// openssl s_client -connect mail.google.com:443 < /dev/null 2>/dev/null | openssl x509 -fingerprint -noout -in /dev/stdin | sed 's/:/ /g'

                                      // The Base64 encoded version of your Gmail login credentials (see below)
const char* credentials = "ZnpvY2N3hcmFAZ21haaWwauY2a9tO1kZyYW5jZXAxMSE=";
// following the shell command for filling up the above variable: 
// echo -n "email.address@gmail.com:password" | base64

const byte led = D5;

// interval in milliseconds from a request to another
int checkInterval = 5000;

// string to search inside the title of the email to mark as alert
String alertString = "down";


// WIFI details
const char* wifiSSID = "Wifi name";
const char* wifiPASS = "Wifi password";

// **************************** //
// DO NOT EDIT BELOW THIS POINT //
// **************************** //

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  pinMode(led, OUTPUT);

  wifiMulti.addAP(wifiSSID, wifiPASS);   // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  Serial.println('\n');
}

void loop() {
  int count = getAlerts();
  if (count == 0) {
    Serial.println("\r\nYou've got no alerts emails");
    digitalWrite(led, LOW);
  } else if (count > 0) {
    Serial.printf("\r\nYou've got %d alerts\r\n", count);
    digitalWrite(led, HIGH);
  } else {
    Serial.println("Could not get alerts mails");
    blinkNTimes(5);
  }
  Serial.println('\n');
  delay(checkInterval);
}

void blinkNTimes(int times){
  while( times-- > 0 ){
    delay(200);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
  }
}


/*int getUnread() {    // a function to get the number of unread emails in your Gmail inbox
  WiFiClientSecure client; // Use WiFiClientSecure class to create TLS (HTTPS) connection
  Serial.printf("Connecting to %s:%d ... \r\n", host, httpsPort);
  if (!client.connect(host, httpsPort)) {   // Connect to the Gmail server, on port 443
    Serial.println("Connection failed");    // If the connection fails, stop and return
    return -1;
  }

  if (client.verify(fingerprint, host)) {   // Check the SHA-1 fingerprint of the SSL certificate
    Serial.println("Certificate matches");
  } else {                                  // if it doesn't match, it's not safe to continue
    Serial.println("Certificate doesn't match");
    return -1;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + 
               "Host: " + host + "\r\n" +
               "Authorization: Basic " + credentials + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n"); // Send the HTTP request headers

  Serial.println("Request sent");

  int unread = -1;

  while (client.connected()) {                          // Wait for the response. The response is in XML format
    client.readStringUntil('<');                        // read until the first XML tag
    String tagname = client.readStringUntil('>');       // read until the end of this tag to get the tag name
    if (tagname == "fullcount") {                       // if the tag is <fullcount>, the next string will be the number of unread emails
      String unreadStr = client.readStringUntil('<');   // read until the closing tag (</fullcount>)
      unread = unreadStr.toInt();                       // convert from String to int
      break;                                            // stop reading
    }                                                   // if the tag is not <fullcount>, repeat and read the next tag
    
    
  }
  Serial.println("Connection closed");

  return unread;                                        // Return the number of unread emails
}*/


int getAlerts() {    // a function to get the number of unread emails in your Gmail inbox
  WiFiClientSecure client; // Use WiFiClientSecure class to create TLS (HTTPS) connection
  Serial.printf("Connecting to %s:%d ... \r\n", host, httpsPort);
  if (!client.connect(host, httpsPort)) {   // Connect to the Gmail server, on port 443
    Serial.println("Connection failed");    // If the connection fails, stop and return
    return -1;
  }

  if (client.verify(fingerprint, host)) {   // Check the SHA-1 fingerprint of the SSL certificate
    Serial.println("Certificate matches");
  } else {                                  // if it doesn't match, it's not safe to continue
    Serial.println("Certificate doesn't match");
    return -1;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + 
               "Host: " + host + "\r\n" +
               "Authorization: Basic " + credentials + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n"); // Send the HTTP request headers

  Serial.println("Request sent");

  int unread = -1;
  int alerts = -1;

  while (client.connected()) {                          // Wait for the response. The response is in XML format
    client.readStringUntil('<');                        // read until the first XML tag
    String tagname = client.readStringUntil('>');       // read until the end of this tag to get the tag name
    //Serial.println(tagname);   
    if (tagname == "fullcount") {                       // if the tag is <fullcount>, the next string will be the number of unread emails
      String unreadStr = client.readStringUntil('<');   // read until the closing tag (</fullcount>)
      unread = unreadStr.toInt();                       // convert from String to int
      //Serial.println(unread);   
      alerts = 0;

      int cycle = unread;
      while(cycle > 0){
        client.readStringUntil('<');
        tagname = client.readStringUntil('>');
        if (tagname == "entry") {
          client.readStringUntil('<');
          tagname = client.readStringUntil('>');
          if (tagname == "title") {
            String title = client.readStringUntil('<');  
            if(title.indexOf(alertString) > 0){
               Serial.println(title);
               alerts++;
            }
          }
          cycle--;
        }
      }
      
      break;                                            // stop reading
    }                                                   // if the tag is not <fullcount>, repeat and read the next tag
    
    
  }
  Serial.println("Connection closed");

  return alerts;                                        // Return the number of unread emails
}
