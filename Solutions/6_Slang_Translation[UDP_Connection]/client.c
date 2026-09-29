#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter sentence: ");
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        close(sockfd);
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(sockfd, buffer, strlen(buffer), 0,
           (const struct sockaddr *)&server_addr, addr_len);

    memset(buffer, 0, BUFFER_SIZE);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                     (struct sockaddr *)&server_addr, &addr_len);
    if (n < 0) {
        perror("Receive error");
    } else {
        buffer[n] = '\0';
        printf("Translated output: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
