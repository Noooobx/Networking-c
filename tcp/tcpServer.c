#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{

    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(lfd, (struct sockaddr *)&server,sizeof server);
    listen(lfd, 1);
    char rBuf[100];
    int n = sizeof client;
    printf("Server is waitng for client\n");
    int confd = accept(lfd, (struct sockaddr *)&client, &n);

    

    recv(confd, rBuf, sizeof rBuf, 0);
    printf("Client : %s", rBuf);
    printf("\n");
    printf("Server : ");
    char sBuf[100];
    fgets(sBuf, sizeof sBuf, stdin);
    send(confd, sBuf, sizeof sBuf, 0);

    close(confd);
    close(lfd);
    return 0;
    
}