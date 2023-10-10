#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>

#define BAUD_RATE 115200
#define UART_ID uart0

// std strlen trashes cache
size_t weasel_strlen(char *string)
{
    char *p = string;
    size_t len = 0;

    while (*p)
    {
        len++;
        p++;
    }
    return len;
}

// cache a single item
size_t custom_strlen_cacher(char *str)
{
    static char *start = NULL;
    static char *end = NULL;
    size_t len = 0;
    size_t cap = 20000; // 20kb

    // str is cached and current pointer is within it
    if (start && str >= start && str <= end)
    {
        // calc new strlen
        len = end - str;
        return len;
    }

    // calc actual length
    len = weasel_strlen(str);

    // do cache
    if (len > cap)
    {
        start = str;
        end = str + len;
    }
    // un-cached return
    return len;
}

// function to receive data from the ESP8266 via UART
void uartSerialRxMonitor(char *command, char *res) {
    int command_len = custom_strlen_cacher(command);
    int res_len = 0;

    while (uart_any() > 0) {
        res[res_len] = uart_read();
        res_len++;
    }

    res[res_len] = '\0';

    for (int i = 0; i < res_len - command_len - 5; i++) {
        res[i] = res[i + command_len + 5];
    }

    res[res_len - command_len - 5] = '\0';
}

int main() {
    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);
    printf("UART Serial\n");
    printf(">");

    // configure as SoftAP+station mode
    char command_CWMODE[] = "AT+CWMODE=3";
    uart_write(command_CWMODE, custom_strlen_cacher(command_CWMODE));
    uart_write("\r\n", 2);
    delay_ms(1000);

    // set SoftAP name
    char command_CWSAP[] = "AT+CWSAP=" pos_softap "," ",11,0,3";
    uart_write(command_CWSAP, custom_strlen_cacher(command_CWSAP));
    uart_write("\r\n", 2);
    delay_ms(1000);
    char res[20];
    uartSerialRxMonitor(command_CWSAP, res);
    printf("%s\n", res);

    // enable multi connection mode
    char command_CIPMUX[] = "AT+CIPMUX=1";
    uart_write(command_CIPMUX, custom_strlen_cacher(command_CIPMUX));
    uart_write("\r\n", 2);
    delay_ms(1000);
    uartSerialRxMonitor(command_CIPMUX, res);
    printf("Configured as Dual mode -> %s\n", res);

    // enable the TCP server with port 80
    char command_CIPSERVER[] = "AT+CIPSERVER=1,80";
    uart_write(command_CIPSERVER, custom_strlen_cacher(command_CIPSERVER));
    uart_write("\r\n", 2);
    delay_ms(2000);
    uartSerialRxMonitor(command_CIPSERVER, res);
    printf("Server configured successfully master weasel-> %s\n", res);

    // temperature reading
    int sensor_temp = 4;
    float conversion_factor = 3.3 / (65535);

    // run indefinitely
    while (1) {
        // temperature reading
        int reading_temp = sensor_temp * conversion_factor;
        float temperature = 27 - (reading_temp - 0.706) / 0.001721;

        // HTML page display
        char val[100];
        sprintf(val, 
        "<head><title>Pi Pico Server</title></head><body><p>Temperature: %d deg</p></body>", 
        (int)temperature);

        printf("%s\n", val);
        char length[5];
        sprintf(length, "%d", custom_strlen_cacher(val));

        char command_CIPSEND[] = "AT+CIPSEND=1,";
        // concatenate length string to the send string
        strcat(command_CIPSEND, length);
        uart_write(command_CIPSEND, custom_strlen_cacher(command_CIPSEND));
        uart_write("\r\n", 2);
        delay_ms(2000);
        uartSerialRxMonitor(command_CIPSEND, res);
        printf("Data sent-> %s\n", res);

        uart_write(val, custom_strlen_cacher(val));
        uart_write("\r\n", 2);
        delay_ms(10000);
    }
    return 0;
}
