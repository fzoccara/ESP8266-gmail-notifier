# ESP8266-gmail-notifier
Arduino program, lighting up a led whenever an email from gmail is unread and with special string in title

Go to https://www.google.com/settings/security/lesssecureapps to enable access for "less secure apps":

The Base64 encoded version of your Gmail login credentials (see below)
following the shell command for filling up the above variable: 
$ echo -n 'email.address@gmail.com:password' | base64

The SHA-1 fingerprint of the SSL certificate for the Gmail server (see below)
following the shell command for filling up the above variable: 
$ openssl s_client -connect mail.google.com:443 < /dev/null 2>/dev/null | openssl x509 -fingerprint -noout -in /dev/stdin | sed 's/:/ /g'

String to search inside the title of the email to mark as alert (down default)

Interval in milliseconds from a request to another (5000 milliseconds default)
