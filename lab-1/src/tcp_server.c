#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "data.h"

#define PORT 8080

static int recv_all(int sock, void *buffer, size_t size) {
    char *data = buffer;
    size_t total = 0;

    while (total < size) {
        ssize_t received = read(sock, data + total, size - total);
        if (received <= 0) {
            return -1;
        }
        total += (size_t)received;
    }

    return 0;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    TaskData received_data;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP Server listening on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    clock_t start_time = clock();
    if (recv_all(new_socket, &received_data, sizeof(received_data)) < 0) {
        perror("read failed");
        close(new_socket);
        close(server_fd);
        return EXIT_FAILURE;
    }

    clock_t end_time = clock();
    double tcp_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Received TCP Data:\n");
    for(int i=0; i<4; i++) printf("Float[%d]: %.2f\n", i, received_data.numbers_float[i]);
    for(int i=0; i<2; i++) printf("Int[%d]: %d\n", i, received_data.numbers_int[i]);
    printf("TCP Transfer Time: %.3f ms\n", tcp_time);

    close(new_socket);
    close(server_fd);
    return 0;
}
