#include <netinet/in.h>
#include <stdio.h>

int main()
{

    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int lfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(lfd, (struct sockaddr *)&server, sizeof server);

    char rBuf[100];
    int n = sizeof(client);

    printf("Server is ready..... waiting for nessage from client!!!!!\n");

    recvfrom(lfd, rBuf, sizeof rBuf, 0, (struct sockaddr *)&client, &n);
    printf("Client : %s\n", rBuf);

    printf("Server : ");
    fgets(rBuf, sizeof rBuf, stdin);

    sendto(lfd, rBuf, sizeof rBuf, 0, (struct sockaddr *)&client, n);
    close(lfd);

    return 0;
}