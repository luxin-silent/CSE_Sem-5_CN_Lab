#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>1

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    const char *abbrev;
    const char *expansion;
} SlangMap;

SlangMap dictionary[] = {
    {"tbh", "to be honest"},
    {"ig", "I guess"},
    {"tbf", "to be fair"},
    {"atm", "at the moment"},
    {"irl", "in real life"},
    {"lol", "laughing out loud"},
    {"asap", "as soon as possible"},
    {"omg", "oh my god"},
    {"ttyl", "talk to you later"},
    {"idk", "I don't care"},
    {"nvm", "never mind"}
};

int dict_size = sizeof(dictionary) / sizeof(dictionary[0]);

void translate_message(const char *input, char *output) {
    char temp[BUFFER_SIZE];
    strcpy(temp, input);
    output[0] = '\0';

    char *token = strtok(temp, " ");
    while (token != NULL) {
        char clean_word[100] = {0};
        char leading_punct[100] = {0};
        char trailing_punct[100] = {0};

        int len = strlen(token);
        int start = 0, end = len - 1;

        while (start < len && ispunct(token[start])) {
            start++;
        }
        strncpy(leading_punct, token, start);

        while (end >= start && ispunct(token[end])) {
            end--;
        }
        strncpy(trailing_punct, token + end + 1, len - 1 - end);

        if (start <= end) {
            strncpy(clean_word, token + start, end - start + 1);
        }

        char lower_word[100] = {0};
        for (int i = 0; clean_word[i]; i++) {
            lower_word[i] = tolower(clean_word[i]);
        }

        const char *replacement = clean_word;
        for (int i = 0; i < dict_size; i++) {
            if (strcmp(lower_word, dictionary[i].abbrev) == 0) {
                replacement = dictionary[i].expansion;
                break;
            }
        }

        strcat(output, leading_punct);
        strcat(output, replacement);
        strcat(output, trailing_punct);
        strcat(output, " ");

        token = strtok(NULL, " ");
    }

    int out_len = strlen(output);
    if (out_len > 0 && output[out_len - 1] == ' ') {
        output[out_len - 1] = '\0';
    }
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char translated[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Translation Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        memset(translated, 0, BUFFER_SIZE);

        int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                         (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("Recvfrom error");
            continue;
        }
        buffer[n] = '\0';

        printf("\n[Received from %s:%d]: %s\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        translate_message(buffer, translated);

        printf("[Sending back]: %s\n", translated);

        sendto(sockfd, translated, strlen(translated), 0,
               (const struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}
