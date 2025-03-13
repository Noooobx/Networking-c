// TCP Multi Chat : Client

#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    struct sockaddr_in server;
    int lfd;
    char rBuf[100],sBuf[100],choice[10];
    lfd=socket(AF_INET,SOCK_STREAM,0);
    server.sin_family=AF_INET;
    server.sin_port=htons(2011);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    printf("\nClient ready....\n");
    connect(lfd,(struct sockaddr *)&server,sizeof server);
    
	while(1){
		memset(rBuf, 0, sizeof rBuf);
		memset(sBuf, 0, sizeof sBuf);

		 printf("\nClient :" );
		 fgets(sBuf,sizeof sBuf,stdin);
		 send(lfd,sBuf,sizeof sBuf,0);

		 recv(lfd,rBuf,sizeof rBuf,0);
		 printf("Server:%s",rBuf);
		 
		 printf("\nDo you like to continue (y/n) :");
		 fgets(choice,sizeof choice,stdin);
		if(strcmp(choice,"n\n")==0)
		{
			strcmp(rBuf,"end\n");
			send(lfd,rBuf,sizeof sBuf,0);
			break;
		}
	  	printf("\n");
   }   
    close(lfd);
   return 0;
}
