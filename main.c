#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024
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

// file reads
char *read_file()
{
    static char buffer[BUFFER_SIZE];
    char *current = buffer;
    // read x bytes at a time - need to benchmark
    int bytes;
    int chunk = 500;

    // TO-DO handle different file types

    FILE *fp = fopen("index.html", "r");
    if (fp)
    {
        do
        {
            bytes = fread(current, sizeof(char), chunk, fp);
            current += bytes;
        } while (bytes == chunk);
        fclose(fp);
        // terminate buffer so string funcs work
        *current = '\0';
        // printf("%s", buffer);
        return buffer;
    }
    return NULL;
}

// func to receive data from the ESP8266 via UART
void uartSerialRxMonitor(char *command, char *res)
{
    int command_len = custom_strlen_cacher(command);
    int res_len = 0;

    while (uart_any() > 0)
    {
        res[res_len] = uart_read();
        res_len++;
    }
    res[res_len] = '\0';

    for (int i = 0; i < res_len - command_len - 5; i++)
    {
        res[i] = res[i + command_len + 5];
    }
    res[res_len - command_len - 5] = '\0';
}

int main()
{
    // init UART
    uart_init(UART_ID, BAUD_RATE);
    printf("UART Serial\n");
    printf(">");
    char res[20];

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

    while (1)
    {
        // HTML page display
        char val[100];
        sprintf(val,
                "<head><title>Pi Pico Server</title></head><body><p>Temperature: %d deg</p></body>",
                (int)69);

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
