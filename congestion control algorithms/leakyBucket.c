#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#define BUCKET_SIZE 10 // Maximum bucket capacity
#define OUT_RATE 3     // Fixed output rate

void leakyBucket(int packets[], int n) {
    int bucket = 0; // Current fill level of bucket

    for (int i = 0; i < n; i++) {
        printf("\nPacket %d arriving: %d packets\n", i + 1, packets[i]);

        // If incoming packets exceed the available bucket space, drop excess
        if (packets[i] + bucket > BUCKET_SIZE) {
            printf("Bucket overflow! Dropping %d packets.\n", (packets[i] + bucket) - BUCKET_SIZE);
            bucket = BUCKET_SIZE; // Bucket is full
        } else {
            bucket += packets[i]; // Add packets to the bucket
        }

        // Process packets at a fixed rate
        printf("Processing packets...\n");
        sleep(10); // Simulate time passing

        if (bucket >= OUT_RATE) {
            printf("Sent %d packets.\n", OUT_RATE);
            bucket -= OUT_RATE;
        } else {
            printf("Sent %d packets.\n", bucket);
            bucket = 0;
        }

        printf("Remaining in bucket: %d packets\n", bucket);
    }

    // Empty remaining packets in bucket
    while (bucket > 0) {
        sleep(10);
        if (bucket >= OUT_RATE) {
            printf("\nSent %d packets.\n", OUT_RATE);
            bucket -= OUT_RATE;
        } else {
            printf("\nSent %d packets.\n", bucket);
            bucket = 0;
        }
        printf("Remaining in bucket: %d packets\n", bucket);
    }
}

int main() {
    int packets[] = {4, 8, 2, 6, 1, 10};
    int n = sizeof(packets) / sizeof(packets[0]);

    printf("Leaky Bucket Simulation:\n");
    leakyBucket(packets, n);

    return 0;
}
