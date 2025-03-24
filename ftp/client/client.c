#include <stdio.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


int main(int argc, char *argv[]){

	if (argc != 3) {
        	printf("Usage: %s <IP Address> <Port>\n", argv[0]);
        	return 1;
    	}

    	char *ip = argv[1];
    	int port = atoi(argv[2]);  // Convert port to integer

    	struct sockaddr_in server;
   	server.sin_family = AF_INET;
    	server.sin_port = htons(port);
    	server.sin_addr.s_addr = inet_addr(ip);
	
	
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	
	connect(lfd,(struct sockaddr *)&server, sizeof server);
	
	char choice[100];
	char sBuf[100] = "";
	memset(sBuf, 0, sizeof(sBuf));
	char rBuf[100] = "";
	memset(rBuf, 0, sizeof(rBuf)); 
	
	FILE *f1;
	
	while(1){
		printf("-----Chose What to do------\n1.get: To acces a file from the server\n2.close: Close the connection.\n");
		scanf("%s", choice);
		getchar();
		
		
		if (strcmp(choice,"get") == 0) {
			printf("Enter the name of the  file: ");
			scanf("%s",sBuf); 
			send(lfd,sBuf,sizeof(sBuf),0); 
			recv(lfd,rBuf,sizeof(rBuf),0);
			printf("The content from the server is : \n");
			f1 = fopen("Answer.txt","w");
			fprintf(f1, "%s", rBuf);
			printf("%s",rBuf);
		}else if(strcmp(choice,"close") == 0){
			printf("inside the close\n");
			close(lfd);
			break;
		}
		
	}

	return 0;
}