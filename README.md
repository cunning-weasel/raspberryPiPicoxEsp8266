# raspberryPiPico x Esp8266
setup of raspberry pi pico (non wifi) connecting up to the ESP8266 12E D1 Mini. 

ESP8266 acts as the access point for picoo to run a c based web server.

For the moment, the MVP should be able to process and store temperature readings - and process this in a multi-threaded way. 

Ultimately, the devices are intended to be used to pick up blackouts, notify users via telegram/ email that there's an electricity cut, and the device should trigger a set of instructions so as to time the blackout, keep a log of this data and predict when and how long the next blackout will be on an ongoing basis. 
