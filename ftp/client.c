#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    // Get server IP and port from command line arguments
    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to FTP server
    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    char command[MAX_BUFFER];
    char filename[MAX_BUFFER];
    char buffer[MAX_BUFFER];
    int bytes_received;

    // Get user input for commands
    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, "get") == 0) {
            printf("Enter filename: ");
            scanf("%s", filename);

            // Send 'get' command to the server
            snprintf(buffer, sizeof(buffer), "get %s", filename);
            send(sockfd, buffer, strlen(buffer), 0);

            // Receive the server's response
            bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);

            // Check if we received an error message or file data
            if (bytes_received > 0) {
                // Check if the message contains an error like "550 File not found"
                if (strncmp(buffer, "550", 3) == 0) {
                    // Server returned an error message, handle it
                    printf("Error: File not found on the server.\n");
                } else if (strncmp(buffer, "226", 3) == 0) {
                    // Transfer completed successfully, no file data received
                    printf("Transfer complete, but no file data was received.\n");
                } else {
                    // We received actual file data, write it to the local file
                    FILE *file = fopen(filename, "wb");
                    if (file != NULL) {
                        fwrite(buffer, 1, bytes_received, file);
                        fclose(file);
                        printf("FILE %s RECEIVED FROM SERVER\n", filename);
                    } else {
                        printf("Error opening file to write\n");
                    }
                }
            }
        }
        else if (strcmp(command, "close") == 0) {
            // Close the connection
            close(sockfd);
            printf("Connection closed\n");
            break;
        }
        else {
            printf("Invalid command!\n");
        }
    }

    return 0;
}
