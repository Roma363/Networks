#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include "data.h"

#define PORT 8081

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    TaskData received_data;
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", PORT);
    len = sizeof(cliaddr);

    clock_t start_time = clock();
    ssize_t received = recvfrom(sockfd, &received_data, sizeof(received_data),
                                0, (struct sockaddr *)&cliaddr, &len);
    if (received < 0) {
        perror("recvfrom failed");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if ((size_t)received != sizeof(received_data)) {
        fprintf(stderr, "Invalid UDP packet size\n");
        close(sockfd);
        return EXIT_FAILURE;
    }

    clock_t end_time = clock();
    double udp_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    printf("Received UDP Data:\n");
    for(int i=0; i<4; i++) printf("Float[%d]: %.2f\n", i, received_data.numbers_float[i]);
    for(int i=0; i<2; i++) printf("Int[%d]: %d\n", i, received_data.numbers_int[i]);
    printf("UDP Receive Time: %.3f ms\n", udp_time);

    close(sockfd);
    return 0;
}
