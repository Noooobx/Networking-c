#include <stdio.h>

#define MAX 1000

void main() {
    int n, i, j, adj[50][50], d;
    
    // Input the number of nodes
    printf("\nEnter the Number of Nodes: ");
    scanf("%d", &n);

    // Input the cost between nodes
    printf("Enter the cost between Nodes:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j)
                adj[i][j] = 0;
            else {
                printf("Cost from %d -> %d: ", i + 1, j + 1);
                scanf("%d", &adj[i][j]);
            }
        }
    }

    int dist[n], visited[n], round, v, src;

    // Initialize distance and visited arrays
    for (i = 0; i < n; i++) {
        dist[i] = MAX;
        visited[i] = 0;
    }

    // Input the source node
    printf("\nEnter the source Node: ");
    scanf("%d", &src);
    src -= 1; // Adjust for 0-based indexing
    dist[src] = 0;

    int nxhop[n]; // Stores next hop information
    for (i = 0; i < n; i++)
        nxhop[i] = 0;

    // Dijkstra's Algorithm
    for (round = 0; round < n - 1; round++) {
        int min = MAX, min_index;

        // Find the minimum distance node
        for (v = 0; v < n; v++) {
            if (visited[v] == 0 && dist[v] < min) {
                min = dist[v];
                min_index = v;
            }
        }

        visited[min_index] = 1;

        // Update the distances of adjacent nodes
        for (d = 0; d < n; d++) {
            if (!visited[d] && adj[min_index][d] && dist[min_index] != MAX &&
                dist[min_index] + adj[min_index][d] < dist[d]) {
                dist[d] = dist[min_index] + adj[min_index][d];

                // Update next hop
                if (min_index != src)
                    nxhop[d] = min_index + 1;
            }
        }

        // Print the initial routing table
        if (min_index == src) {
            printf("\nRouting Table of Node %d", src + 1);
            printf("\nDestination\tCost\tNext Hop\n");
            for (i = 0; i < n; i++) {
                if (dist[i] == 0)
                    printf("%d\t\t-\t-\n", i + 1);
                else
                    printf("%d\t\t%d\t-\n", i + 1, dist[i]);
            }
        }
    }

    // Final Routing Table
    printf("\n\nAfter Applying Dijkstra's Algorithm:\n");
    printf("\nRouting Table of Node %d", src + 1);
    printf("\nDestination\tCost\tNext Hop\n");
    for (i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, dist[i]);
        if (nxhop[i] == 0)
            printf("-\n");
        else
            printf("%d\n", nxhop[i]);
    }

    // Print the shortest path costs
    for (i = 1; i < n; i++)
        printf("The cost of the shortest path from router %d to %d is %d\n",
               src + 1, i + 1, dist[i]);
}
