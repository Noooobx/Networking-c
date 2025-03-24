#include <stdio.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

	struct sockaddr_in server,client;
	
	server.sin_family = AF_INET;
	server.sin_port = htons(2006);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	
	bind(lfd,(struct sockaddr *)&server,sizeof server);
	
	listen(lfd,1);
	
	socklen_t n = sizeof client;
	
	int confd = accept(lfd,(struct sockaddr *)&client,&n);
	
	
	char sBuf[100];
	char rBuf[100];
	memset(sBuf,0,sizeof sBuf);
	memset(rBuf,0,sizeof rBuf);
	FILE *f1;
	while(1){
		recv(confd,sBuf,sizeof sBuf,0);
		f1 = fopen(sBuf,"r");
		
		int i = 0;
		
		while ((rBuf[i] = fgetc(f1)) != EOF && i < sizeof(rBuf) - 1) {
			i++;
		}
		rBuf[i] = '\0';
		fclose(f1);
		printf("%s",rBuf);
		send(confd,rBuf,sizeof(rBuf),0);
	}
	close(lfd);
	return 0;
}