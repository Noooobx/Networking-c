
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

void swap(int* arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
      
        // Last i elements are already in place, so the loop
        // will only num n - i - 1 times
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(arr, j, j + 1);
        }
    }
}

int main()
{
    struct sockaddr_in client,server;
    struct data {
        int size;
        int numbers[10];
    } d;

    int lfd,n,confd;
    char rBuf[100],sBuf[100],choice;
    pid_t childpid;	
    
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
				 recv(confd,&d,sizeof d,0);
				 printf("Client:%d",d.size);
                 printf("\nReceived Array\n");
                 for(int i = 0 ;i < d.size; i++){
                    printf("%d ",d.numbers[i]);
                 }
                 bubbleSort(d.numbers, d.size);

                 printf("\nSorted array sent to client succesfully.....\n");
                 send(confd,&d,sizeof d,0);
                 printf("\n");
				 
		 	}		         
		}
        
		 close(confd);
    }
    exit (0);
}
