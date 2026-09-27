#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "data.h"

#define PORT 8080

static int send_all(int sock, const void *buffer, size_t size) {
    const char *data = buffer;
    size_t total = 0;

    while (total < size) {
        ssize_t sent = send(sock, data + total, size - total, 0);
        if (sent <= 0) {
            return -1;
        }
        total += (size_t)sent;
    }

    return 0;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    TaskData data_to_send;
    data_to_send.numbers_float[0] = 1.1f; data_to_send.numbers_float[1] = 2.2f;
    data_to_send.numbers_float[2] = 3.3f; data_to_send.numbers_float[3] = 4.4f;
    data_to_send.numbers_int[0] = 100;    data_to_send.numbers_int[1] = 200;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    if (send_all(sock, &data_to_send, sizeof(data_to_send)) < 0) {
        perror("send failed");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("TCP data sent successfully.\n");

    close(sock);
    return 0;
}
