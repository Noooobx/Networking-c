#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5557
#define MAX_BUFFER 1024
#define FILE_STORAGE_DIR "./" // Directory where files are stored

// Function to handle client requests
void handle_client(int client_sockfd) {
    char buffer[MAX_BUFFER];
    char response[MAX_BUFFER];
    int bytes_received;

    // Send a welcome message
    snprintf(response, sizeof(response), "220 Welcome to the Simple FTP Server\r\n");
    send(client_sockfd, response, strlen(response), 0);

    while (1) {
        bzero(buffer, sizeof(buffer));
        bytes_received = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            break; // Connection closed or error
        }

        buffer[bytes_received] = '\0'; // Null-terminate the received data

        // Handle the 'get' command (RETR)
        if (strncmp(buffer, "get", 3) == 0) {
            char filename[MAX_BUFFER];
            sscanf(buffer, "get %s", filename);
            snprintf(response, sizeof(response), "150 Opening data connection for %s\r\n", filename);
            send(client_sockfd, response, strlen(response), 0);

            // Open the requested file
            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                snprintf(response, sizeof(response), "550 File not found\r\n");
                send(client_sockfd, response, strlen(response), 0);  // Send the error message
            } else {
                // Read and send the file content only if the file is found
                while (1) {
                    int bytes_read = fread(buffer, 1, sizeof(buffer), file);
                    if (bytes_read > 0) {
                        send(client_sockfd, buffer, bytes_read, 0);
                    }
                    if (bytes_read < sizeof(buffer)) {
                        break; // End of file
                    }
                }
                fclose(file);
            
                // Send response after file transfer
                snprintf(response, sizeof(response), "226 Transfer complete\r\n");
                send(client_sockfd, response, strlen(response), 0);
            }
            
        } 
        // Handle invalid or unsupported commands
        else {
            snprintf(response, sizeof(response), "500 Syntax error, command unrecognized\r\n");
            send(client_sockfd, response, strlen(response), 0);
        }
    }

    close(client_sockfd); // Close the client connection
}

int main() {
    int server_sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(server_sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sockfd, 5) < 0) {
        perror("Error listening on socket");
        close(server_sockfd);
        exit(1);
    }

    printf("FTP server listening on port %d...\n", PORT);

    // Accept incoming client connections
    while (1) {
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("Error accepting connection");
            continue; // Try to accept other clients
        }

        printf("Client connected, handling client...\n");

        // Handle client commands (e.g., get command)
        handle_client(client_sockfd);

        printf("Client connection closed.\n");
    }

    close(server_sockfd); // Close the server socket
    return 0;
}
