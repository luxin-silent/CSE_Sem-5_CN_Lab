#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 256

void *handle_client(void *client_socket_ptr) {
    int client_fd = *(int *)client_socket_ptr;
    free(client_socket_ptr);

    int N;
    if (recv(client_fd, &N, sizeof(int), 0) <= 0) {
        close(client_fd);
        pthread_exit(NULL);
    }

    int matrix[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            recv(client_fd, &matrix[i][j], sizeof(int), 0);
        }
    }

    int is_upper = 1;
    int is_lower = 1;
    int is_diagonal = 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i > j && matrix[i][j] != 0) {
                is_upper = 0;
            }
            if (i < j && matrix[i][j] != 0) {
                is_lower = 0;
            }
            if (i != j && matrix[i][j] != 0) {
                is_diagonal = 0;
            }
        }
    }

    char response[BUFFER_SIZE];
    if (is_diagonal) {
        strcpy(response, "Diagonal Matrix");
    } else if (is_upper) {
        strcpy(response, "Upper Triangular Matrix");
    } else if (is_lower) {
        strcpy(response, "Lower Triangular Matrix");
    } else {
        strcpy(response, "Neither Upper, Lower, nor Diagonal Matrix");
    }

    send(client_fd, response, strlen(response) + 1, 0);

    close(client_fd);
    pthread_exit(NULL);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        int *new_socket = malloc(sizeof(int));
        *new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        if (*new_socket < 0) {
            perror("Accept failed");
            free(new_socket);
            continue;
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_socket) != 0) {
            perror("Thread creation failed");
            free(new_socket);
        } else {
            pthread_detach(thread_id);
        }
    }

    close(server_fd);
    return 0;
}
