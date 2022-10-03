#include <stdio.h>
#include "pico/stdlib.h"

int main() {

    // Initialize chosen serial port
    stdio_init_all();

    const uint led_pin25 = 25;
    const uint led_pin14 = 14;

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
