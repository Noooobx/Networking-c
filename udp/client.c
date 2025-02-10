#include <netinet/in.h>
#include <stdio.h>

int main()
{
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int lfd = socket(AF_INET, SOCK_DGRAM, 0);

    char sBuf[100];
    printf("client : ");
    fgets(sBuf, sizeof sBuf, stdin);

    int n = sizeof server;
    sendto(lfd, sBuf, sizeof sBuf, 0, (struct sockaddr *)&server, n);

    int rBuf[100];

    recvfrom(lfd, rBuf, sizeof rBuf, 0, (struct sockaddr *)&server, &n);
    printf("Server : %s\n", rBuf);
    close(lfd);

    return 0;
}