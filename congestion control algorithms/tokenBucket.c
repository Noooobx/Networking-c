#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

#define BUCKET_SIZE 10  // Maximum bucket capacity
#define TOKEN_RATE 3    // Tokens added per time unit

void tokenBucket(int packets[], int n) {
    int tokens = 0; // Initial token count

    for (int i = 0; i < n; i++) {
        // Add new tokens (capped at BUCKET_SIZE)
        tokens += TOKEN_RATE;
        if (tokens > BUCKET_SIZE)
            tokens = BUCKET_SIZE;

        printf("\nPacket %d arriving: %d packets\n", i + 1, packets[i]);

        // Check if there are enough tokens to send the packet
        if (packets[i] <= tokens) {
            printf("Packet sent! Used %d tokens.\n", packets[i]);
            tokens -= packets[i];
        } else {
            printf("Not enough tokens! Dropping packet (needed %d, but had %d).\n", packets[i], tokens);
        }

        printf("Remaining tokens: %d\n", tokens);
        sleep(1); // Simulate time delay
    }
}

int main() {
    int packets[] = {4, 8, 2, 6, 1, 10}; // Incoming packet sizes
    int n = sizeof(packets) / sizeof(packets[0]);

    printf("Token Bucket Simulation:\n");
    tokenBucket(packets, n);

    return 0;
}
