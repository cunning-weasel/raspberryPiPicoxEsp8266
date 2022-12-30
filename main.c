#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "hardware/regs/dreq.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Function for configuring the ESP8266 as a SoftAP+station
void config_esp8266(int sockfd)
{
    char buffer[256];
    int n;

    // Configure the ESP8266 as a SoftAP+station
    sprintf(buffer, "AT+CWMODE=3\r\n");
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return;
    }
    usleep(1000000); // Wait 1 second

    // Set the SoftAP name
    sprintf(buffer, "AT+CWSAP=\"pos_softap\",\"\",11,0,3\r\n");
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return;
    }
    usleep(1000000); // Wait 1 second

    // Enable multi-connection mode
    sprintf(buffer, "AT+CIPMUX=1\r\n");
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return;
    }
    usleep(1000000); // Wait 1 second
}

// Main function
int main(void)
{
    // Socket descriptor and new socket descriptor
    int sockfd, new_sockfd;
    // Port number and client length
    int portno, clilen;
    // Server and client addresses
    struct sockaddr_in serv_addr, cli_addr;
    // Buffer for storing data
    char buffer[256];
    // Number of bytes read or written
    int n;

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        return 1;
    }

    // Read the temperature sensor value
    int sensor_temp = 4;
    int conversion_factor = 3.3 / 65535;
    int reading_temp = read_u16(sensor_temp) * conversion_factor;
    float temperature = 27 - (reading_temp - 0.706) / 0.001721;

    // Send the temperature value in an HTML page to the client
    char val[256];
    sprintf(val, "<head><title>Pi Pico Server</title></head><body><p>Temperature: %d deg</p></body>", (int)temperature);
    int length = strlen(val);
    sprintf(buffer, "AT+CIPSEND=1,%d\r\n", length);
    n = write(new_sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return 1;
    }
    bzero(buffer, 256);
    n = read(new_sockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        return 1;
    }
    printf("Data sent: %s\n", buffer);
    n = write(new_sockfd, val, strlen(val));
    if (n < 0)
    {
        perror("ERROR writing to socket");
        return 1;
    }
}