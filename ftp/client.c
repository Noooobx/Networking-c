// Connect to FTP server on port 21
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(21); // FTP uses port 21 by default
server.sin_addr.s_addr = inet_addr("server_ip"); // replace with actual FTP server IP
int con_status = connect(sockfd, (struct sockaddr*)&server, sizeof(server));

if (con_status < 0) {
    perror("Connection failed");
    exit(0);
}

// Send FTP commands (e.g., USER, PASS) to authenticate
send(sockfd, "USER username\r\n", strlen("USER username\r\n"), 0);
send(sockfd, "PASS password\r\n", strlen("PASS password\r\n"), 0);

// Request a file using RETR command
send(sockfd, "RETR filename\r\n", strlen("RETR filename\r\n"), 0);

// Receive the file content
char buffer[1024];
int bytes_received;
while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
    // Process the received file content (e.g., save to disk)
}

close(sockfd);
