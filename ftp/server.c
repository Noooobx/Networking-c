#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 21
#define MAX_BUFFER 1024

// Simple file storage path
#define FILE_STORAGE_DIR "./"

int handle_client(int control_sockfd) {
    char buffer[MAX_BUFFER];
    char response[MAX_BUFFER];
    int bytes_received;

    // Send initial welcome message
    snprintf(response, sizeof(response), "220 Welcome to the Simple FTP Server\r\n");
    send(control_sockfd, response, strlen(response), 0);

    // Handle commands from the client
    while (1) {
        bzero(buffer, sizeof(buffer));
        bytes_received = recv(control_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break; // Connection closed or error
        }
        
        buffer[bytes_received] = '\0'; // Null terminate the received data

        printf("Received command: %s\n", buffer);

        // Handle USER command
        if (strncmp(buffer, "USER", 4) == 0) {
            snprintf(response, sizeof(response), "331 User name okay, need password\r\n");
            send(control_sockfd, response, strlen(response), 0);
        }
        // Handle PASS command
        else if (strncmp(buffer, "PASS", 4) == 0) {
            snprintf(response, sizeof(response), "230 User logged in, proceed\r\n");
            send(control_sockfd, response, strlen(response), 0);
        }
        // Handle RETR command to get a file
        else if (strncmp(buffer, "RETR", 4) == 0) {
            // Extract the filename from the command
            char filename[MAX_BUFFER];
            sscanf(buffer, "RETR %s", filename);

            snprintf(response, sizeof(response), "150 Opening data connection for %s\r\n", filename);
            send(control_sockfd, response, strlen(response), 0);

            // Open the requested file
            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                snprintf(response, sizeof(response), "550 File not found\r\n");
                send(control_sockfd, response, strlen(response), 0);
            } else {
                // Read and send the file content
                while (1) {
                    int bytes_read = fread(buffer, 1, sizeof(buffer), file);
                    if (bytes_read > 0) {
                        send(control_sockfd, buffer, bytes_read, 0);
                    }
                    if (bytes_read < sizeof(buffer)) {
                        break; // End of file
                    }
                }
                fclose(file);

                // Send response after file transfer
                snprintf(response, sizeof(response), "226 Transfer complete\r\n");
                send(control_sockfd, response, strlen(response), 0);
            }
        }
        // Handle invalid or unsupported commands
        else {
            snprintf(response, sizeof(response), "500 Syntax error, command unrecognized\r\n");
            send(control_sockfd, response, strlen(response), 0);
        }
    }

    return 0;
}

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MAX_BUFFER];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("Error listening on socket");
        close(sockfd);
        exit(1);
    }

    printf("FTP server listening on port %d...\n", PORT);

    // Accept incoming client connections
    while (1) {
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (new_sockfd < 0) {
            perror("Error accepting connection");
            continue; // Try to accept other clients
        }

        printf("Client connected, handling client...\n");

        // Handle client commands (user authentication, file transfer)
        handle_client(new_sockfd);

        // Close the client connection
        close(new_sockfd);
        printf("Client connection closed.\n");
    }

    close(sockfd); // Close the server socket
    return 0;
}
