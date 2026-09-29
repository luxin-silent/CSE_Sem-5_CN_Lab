#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char user_input[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // 1. Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 3. Prompt user for string input
    printf("Enter command (e.g. 'time pls'): ");
    if (fgets(user_input, sizeof(user_input), stdin) != NULL) {
        // Remove trailing newline character
        user_input[strcspn(user_input, "\n")] = 0;
    }

    // 4. Send request string to server
    sendto(sockfd, user_input, strlen(user_input), 0,
           (struct sockaddr *)&server_addr, addr_len);

    // 5. Receive response from server
    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr *)&server_addr, &addr_len);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server Response:\n%s", buffer);
    } else {
        printf("Failed to receive response from server.\n");
    }

    // 6. Close socket
    close(sockfd);
    return 0;
}
