#define debug true    // enable / disable serial debugging

String prefix = "fz-";
String device = prefix + String(ESP.getChipId());

// Go to https://www.google.com/settings/security/lesssecureapps to enable access for "less secure apps":

// The Base64 encoded version of your Gmail login credentials (see below)
// following the shell command for filling up the above variable: 
// $ echo -n 'email.address@gmail.com:password' | base64
char credentials[100];

// The SHA-1 fingerprint of the SSL certificate for the Gmail server (see below)
// following the shell command for filling up the above variable: 
// $ openssl s_client -connect mail.google.com:443 < /dev/null 2>/dev/null | openssl x509 -fingerprint -noout -in /dev/stdin | sed 's/:/ /g'
char fingerprint[60] = "F2 CD E3 BA 74 93 85 31 F5 73 9D 15 00 38 8F F0 C5 7C 79 BE";

// string to search inside the title of the email to mark as alert
char alertString[60] = "down";

char host[40] = "mail.google.com"; // the Gmail server
char url[60] = "/mail/feed/atom"; // the Gmail feed url
char httpsPort[6] = "443"; // the port to connect to the email server

// interval in milliseconds from a request to another
char checkInterval[6] = "5000";

// how many failed request are allowed before alert
char failedTries[3] = "5";

/* Service variables */
int i, count, unread, alerts, cycle, tempFailedTries;
String tagname, unreadStr, title;
bool shouldSaveConfig = false;

/* Configuration for check email timer */
unsigned long checkTimer,  //some global variables available anywhere in the program
  currentMillis;

/* Configuration for reset long press button duration */
unsigned long buttonTimer = 0,
  longPressTime = 5000;// interval for reset eeprom (milliseconds)

//callback notifying us of the need to save config
void saveConfigCallback () {
  #if defined(debug) && debug == true
  Serial.println("Should save config");
  #endif
  shouldSaveConfig = true;
}

