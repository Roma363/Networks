#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "data.h"

#define PORT 8081

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    TaskData data_to_send;
    data_to_send.numbers_float[0] = 1.1f; data_to_send.numbers_float[1] = 2.2f;
    data_to_send.numbers_float[2] = 3.3f; data_to_send.numbers_float[3] = 4.4f;
    data_to_send.numbers_int[0] = 100;    data_to_send.numbers_int[1] = 200;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sockfd);
        return EXIT_FAILURE;
    }

    ssize_t sent = sendto(sockfd, &data_to_send, sizeof(data_to_send),
                          0, (const struct sockaddr *)&servaddr,
                          sizeof(servaddr));
    if (sent < 0) {
        perror("sendto failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if ((size_t)sent != sizeof(data_to_send)) {
        fprintf(stderr, "Incomplete UDP packet sent\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("UDP data sent successfully.\n");

    close(sockfd);
    return 0;
}
