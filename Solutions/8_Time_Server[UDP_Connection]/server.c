#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    signal(SIGCHLD, SIG_IGN);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Time Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) continue;

        buffer[bytes_received] = '\0';

        pid_t pid = fork();

        if (pid == 0) {
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);

            printf("[CLIENT %s:%d] Received Message: \"%s\"\n", client_ip, client_port, buffer);

            if (strcmp(buffer, "time pls") == 0) {
                time_t raw_time;
                time(&raw_time);
                char *time_str = ctime(&raw_time);

                sendto(sockfd, time_str, strlen(time_str), 0,
                       (struct sockaddr *)&client_addr, addr_len);

                printf("[CLIENT %s:%d] Sent Time Response.\n", client_ip, client_port);
            } else {
                char *err_msg = "Invalid Command! Use 'time pls'\n";
                sendto(sockfd, err_msg, strlen(err_msg), 0,
                       (struct sockaddr *)&client_addr, addr_len);

                printf("[CLIENT %s:%d] Sent Error Response.\n", client_ip, client_port);
            }

            exit(0);
        }
    }

    close(sockfd);
    return 0;
}
