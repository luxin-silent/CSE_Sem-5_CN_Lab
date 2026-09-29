#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_zombie(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    signal(SIGCHLD, handle_zombie);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Server started on port %d. Waiting for clients...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        pid_t pid = fork();

        if (pid == 0) {
            close(server_fd);

            pid_t child_pid = getpid();
            char filename[BUFFER_SIZE] = {0};
            char buffer[BUFFER_SIZE] = {0};

            recv(client_fd, filename, sizeof(filename), 0);
            printf("[SERVER (PID: %d)] Client requested file: '%s'\n", child_pid, filename);

            FILE *file = fopen(filename, "r");

            if (file != NULL) {
                snprintf(buffer, sizeof(buffer), "SUCCESS|PID: %d|File found! Reading contents below:\n", child_pid);
                send(client_fd, buffer, strlen(buffer), 0);

                size_t bytes_read;
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                    send(client_fd, buffer, bytes_read, 0);
                }

                fclose(file);
                printf("[SERVER (PID: %d)] File successfully sent to client.\n", child_pid);
            } else {
                snprintf(buffer, sizeof(buffer), "ERROR|PID: %d|Error: Requested file '%s' was not found on the server.\n", child_pid, filename);
                send(client_fd, buffer, strlen(buffer), 0);
                printf("[SERVER (PID: %d)] File not found. Sent error message.\n", child_pid);
            }

            close(client_fd);
            exit(0);
        } else if (pid > 0) {
            close(client_fd);
        } else {
            perror("Fork failed");
        }
    }

    close(server_fd);
    return 0;
}
