#include <stdio.h>
#include <netinet/in.h>

int main()
{

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(lfd, (struct sockaddr *)&server, sizeof server);

    char sBuf[100];
    printf("Client : \n");
    fgets(sBuf, sizeof sBuf, stdin);
    send(lfd, sBuf, sizeof sBuf, 0);

    char rBuf[100];
    recv(lfd, rBuf, sizeof rBuf, 0);
    printf("Server: %s",rBuf);

    return 0;
}