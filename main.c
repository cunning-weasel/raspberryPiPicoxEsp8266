#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "hardware/regs/dreq.h"

#define UART_ID uart0
#define BAUD_RATE 115200

int main()
{
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // configure the ESP8266 in Soft AP+ Station mode
    // using the command "AT+CWMODE=3" - sent through
    // the UART of Pico
    char send[] = "AT+CWMODE=3";
}

// https://raspberrypi.github.io/pico-sdk-doxygen/index.html