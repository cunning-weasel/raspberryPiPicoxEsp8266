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

    // Initialize LED pin
    gpio_init(led_pin25);
    gpio_set_dir(led_pin25, GPIO_OUT);
    gpio_init(led_pin14);
    gpio_set_dir(led_pin14, GPIO_OUT);

    // Loop forever
    while (true) {

        // Blink LED
        // printf("Blinking!\r\n");
        gpio_put(led_pin25, 1);
        gpio_put(led_pin14, 0);
        sleep_ms(500);
        gpio_put(led_pin25, 0);
        gpio_put(led_pin14, 1);
        sleep_ms(900);
    }
}
// https://raspberrypi.github.io/pico-sdk-doxygen/index.html
