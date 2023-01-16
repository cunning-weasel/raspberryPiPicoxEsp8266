# raspberryPiPico x Esp8266
setup of raspberry pi pico w & ESP8266 12E D1 Mini. 

Set up the ESP8266:

Flash the ESP8266 with NodeMCU firmware.
Connect the ESP8266 to network and obtain its IP address.
Configure the UART communication on the ESP8266, set the baud rate and use the software serial library to communicate over UART.
Write the js code that will run on the ESP8266 to handle the Telegram bot functionality. This code should use the Telegram Bot API to interact with the Telegram server, handle incoming messages and commands, and send them to the Raspberry Pi Pico over UART.

Set up the Raspberry Pi Pico:

Connect the Raspberry Pi Pico to network.
Configure the UART communication on the Raspberry Pi Pico, set the baud rate and use the RP2040 Microcontroller Firmware for the UART communication.
Write the C code that will run on the Raspberry Pi Pico to handle the task management functionality. This code should handle the storage of commands, send reminders, and handle tagging other users.

Connect the ESP8266 and the Raspberry Pi Pico:

~~Connect the ESP8266 and the Raspberry Pi Pico via jumper cables on a breadboard.~~
Have the ESP8266 forward incoming commands from the Telegram bot to the Raspberry Pi Pico over UART.
Have the Raspberry Pi Pico send reminders and other commands back to the ESP8266 over UART to be sent to the Telegram bot.


