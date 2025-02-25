#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define port 4330

int main(){

    // TCP socket.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    int len = sizeof(server);

    int con_status = connect(sockfd, (struct sockaddr*)&server, len);

    if(con_status<0){
        perror("coneenciton failed");
        exit(0);
    }

    char filename[512];

    // Sets the enitire array to 0.
    bzero(filename,512);
    printf("Enter the file name :");

    scanf("%s",filename);

    // Send filename and lenght of the array to the server.
    send(sockfd, filename, strlen(filename), 0);

    char output[2048];
    // Sets the enitire array to 0.
    bzero(output, 2048);

    recv(sockfd, output, 2048, 0);
    printf("\n\n------File Content------\n\n%s\n\n------File Content------\n\n",output);
    close(sockfd);

    return 0;
}
