#include <stdio.h>
#include <string.h>
#include <pico/uart.h>

// #define UART_ID uart0
#define BAUD_RATE 115200

// Function to receive data from the ESP8266 via UART
void uartSerialRxMonitor(char *command, char *res)
{
    int command_len = strlen(command);
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
    // Initialize UART
    uart_init(UART_TX, UART_RX, BAUD_RATE);

    printf("UART Serial\n");
    printf(">");

    // configure as SoftAP+station mode
    char send[] = "AT+CWMODE=3";
    uart_write(send, strlen(send));
    uart_write("\r\n", 2);
    delay_ms(1000);

    // Set SoftAP name
    char send[] = "AT+CWSAP=" pos_softap "," ",11,0,3";
    uart_write(send, strlen(send));
    uart_write("\r\n", 2);
    delay_ms(1000);
    char res[20];
    uartSerialRxMonitor(send, res);
    printf("%s\n", res);

    // enable multi connection mode
    char send[] = "AT+CIPMUX=1";
    uart_write(send, strlen(send));
    uart_write("\r\n", 2);
    delay_ms(1000);
    uartSerialRxMonitor(send, res);
    printf("Configured as Dual mode -> %s\n", res);

    // Enable the TCP server with port 80
    char send[] = "AT+CIPSERVER=1,80";
    uart_write(send, strlen(send));
    uart_write("\r\n", 2);
    delay_ms(2000);
    uartSerialRxMonitor(send, res);
    printf("Server configured successfully-> %s\n", res);

    // temperature reading
    int sensor_temp = 4;
    float conversion_factor = 3.3 / (65535);

    // Here the code runs indefinitely
    while (1)
    {
        // temperature reading
        int reading_temp = sensor_temp.read_u16() * conversion_factor;
        float temperature = 27 - (reading_temp - 0.706) / 0.001721;

        // Place basic code for HTML page display
        char val[100];
        sprintf(val, 
            "<head>
                <title>Pi Pico Server</title>
            </head>
            <body>
                <p>Temperature: %d deg</p>
            </body>", 
            (int)temperature);

        printf("%s\n", val);
        char length[5];
        sprintf(length, "%d", strlen(val));

        char send[] = "AT+CIPSEND=1,";
        // concatenate length string to the send string
        strcat(send, length);
        uart_write(send, strlen(send));
        uart_write("\r\n", 2);
        delay_ms(2000);
        uartSerialRxMonitor(send, res);
        printf("Data sent-> %s\n", res);

        uart_write(val, strlen(val));
        uart_write("\r\n", 2);
        delay_ms(10000);
    }
    return 0;
}
