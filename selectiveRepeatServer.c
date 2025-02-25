#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_PKT 12  // Total number of packets

int main() {
    int sockfd, port = 8080, window_size, total_packets = MAX_PKT;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
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

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    // Get window size from the client
    recvfrom(sockfd, &window_size, sizeof(window_size), 0, (struct sockaddr *)&client_addr, &client_len);
    printf("The window size obtained from Client is: %d\n", window_size);

    // Send total number of packets to client
    sendto(sockfd, &total_packets, sizeof(total_packets), 0, (struct sockaddr *)&client_addr, client_len);
    printf("Sending total number of packets\n");

    int frame = 0;
    while (frame < total_packets / window_size) {
        printf("Initializing the transmit buffer\n");
        printf("The frame to be sent is %d with packets: ", frame);

        for (int i = 0; i < window_size; i++) {
            int pkt_num = frame * window_size + i;
            printf("%d ", pkt_num);
        }
        printf("\nsending frame %d\n", frame);

        // Send packets in the frame
        for (int i = 0; i < window_size; i++) {
            int pkt_num = frame * window_size + i;
            sendto(sockfd, &pkt_num, sizeof(pkt_num), 0, (struct sockaddr *)&client_addr, client_len);
        }

        // Wait for acknowledgment
        for (int i = 0; i < window_size; i++) {
            int ack;
            recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, &client_len);
            if (ack == -1) {
                int retransmit_pkt = frame * window_size + i;
                printf("Negative acknowledgement received for packet:%d\n", retransmit_pkt);
                printf("Retransmitting packet:%d\n", retransmit_pkt);
                sendto(sockfd, &retransmit_pkt, sizeof(retransmit_pkt), 0, (struct sockaddr *)&client_addr, client_len);
            }
        }

        frame++;
    }

    printf("All frames sent successfully\n");
    close(sockfd);
    return 0;
}

