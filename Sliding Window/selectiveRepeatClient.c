#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_PKT 12  // Total number of packets

int main() {
    int sockfd, port = 8080, window_size, total_packets;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Send request to the server
    printf("sending request to the server\n");
    printf("Waiting for reply\n");

    // Get window size from user and send it to the server
    printf("Enter the window size: ");
    scanf("%d", &window_size);
    sendto(sockfd, &window_size, sizeof(window_size), 0, (struct sockaddr *)&server_addr, server_len);
    printf("sending window size\n");

    // Wait for total number of packets from server
    recvfrom(sockfd, &total_packets, sizeof(total_packets), 0, (struct sockaddr *)&server_addr, &server_len);
    printf("Total packets to receive from SERVER: %d\n", total_packets);
    printf("starting the process of receiving\n");

    int frame = 0;
    while (frame < total_packets / window_size) {
        printf("Initializing the received buffer\n");
        printf("waiting for the frame\n");

        int pkt_num;
        printf("The received frame is %d with packets: ", frame);
        for (int i = 0; i < window_size; i++) {
            recvfrom(sockfd, &pkt_num, sizeof(pkt_num), 0, (struct sockaddr *)&server_addr, &server_len);
            printf("%d ", pkt_num);
            // Simulate negative acknowledgment
            int ack;
            printf("\nenter -1 to send negative acknowledgement for the following packets\n");
            printf("packet:%d\n", pkt_num);
            scanf("%d", &ack);
            sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, server_len);
        }

        frame++;
    }

    printf("all frames received successfully\n");
    close(sockfd);
    return 0;
}

