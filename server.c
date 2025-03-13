// TCP Multi Chat : Server

#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

void toUpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main()
{
    struct sockaddr_in client,server;
    int lfd,n,confd;
    char rBuf[100],sBuf[100],choice;
    pid_t childpid;	//
    
    lfd=socket(AF_INET,SOCK_STREAM,0); 
    server.sin_family=AF_INET;
    server.sin_port=htons(2011);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(lfd,(struct sockaddr *)&server,sizeof server);	
    listen(lfd,5); //queue  of pending  connections is 5
    
	while(1){
		 printf("\nServer ready,waiting for client....\n");    
		 n=sizeof client;
		 confd=accept(lfd,(struct sockaddr *)&client,&n);

         // If a child process is created succesfully.
		 if ((childpid = fork()) == 0) {
			close(lfd);
			 while(1){
                // Sets the rBuf array to fully zeros.
				 memset(rBuf, 0, sizeof rBuf);
				 recv(confd,rBuf,sizeof rBuf,0);
				 printf("Client:%s",rBuf);
				 if(strcmp(rBuf,"end\n") == 0)
				 	break;
				 toUpperCase(rBuf);
				 
				 printf("Server:%s",rBuf);
				 send(confd,rBuf,sizeof rBuf,0);
				 printf("\n");
		 	}		         
		}
		 close(confd);
    }
    exit (0);
}
