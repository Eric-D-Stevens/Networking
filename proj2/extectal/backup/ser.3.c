/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main()
{
	int sock, connected, bytes_recieved , true = 1;  
	char send_data [1024] , recv_data[1024];       

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}

	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);


	while(1)
	{  

		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		printf("\n I got a connection from (%s , %d)",
				inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		fflush(stdout);
		while (1)
		{


			bytes_recieved = recv(connected,recv_data,1024,0);
			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "-l") == 0)
			{
				char command[50];
				strcpy(command, "ls > temp1");
				system(command); 
				SendData(connected);
				strcpy(command, "rm -f temp1");
				system(command); 
			}


			else  if (recv_data[0] == '-' && recv_data[1] == 'g')
			{
				char data[100];
				sprintf(data, "%s", recv_data);
				char *filename;
				filename = strtok(data, " ");
				filename = strtok(NULL, " ");
				if(access(filename, F_OK) == -1)
				{
					printf("\nFile %s does not exist\n", filename);
					fflush(stdout);
					break;
				}

				printf("understood g command: %s", filename);
				fflush(stdout);
				
				
				char command[50];
				 
				sprintf(&command, "cat %s > temp1", filename);
				printf("\n\n %s \n\n", command);
				system(command); 


				SendData(connected);
				strcpy(command, "rm -f temp1");
				system(command); 
			}


			else
			{ 
				printf("\n RECIEVED DATA = %s " , recv_data);
				fflush(stdout);
			}
		}
	}       

	close(sock);
	return 0;
}




int SendData(int connected){

	int tempcon = connected;	
	//////
	connected = 5001;
	//////


	int sock, bytes_recieved , true = 1;  
	char send_data [1024] , recv_data[1024];       

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5001);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}


	printf("\nTCPServer Waiting for client on port 5001");
	fflush(stdout);

	char redmsg[20];
	strcpy(redmsg,"read on port 5001");
	send(tempcon, &redmsg, 1, 0);  

	sin_size = sizeof(struct sockaddr_in);

	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

	printf("\n I got a connection from (%s , %d)",
			inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	fflush(stdout);

	FILE *fp;
	char *filename = "temp1";
	int c;
	int n = 0;

	fp = fopen(filename,"r");
	if(fp == NULL){
		perror("fail to open temp");
	}
				
	do{
		c = fgetc(fp);
		if(feof(fp)){break;}
		char ch = (char) c;
		send(connected, &ch, 1, 0);  
	}while(1);	
	fclose(fp);
	char eof[3];
	eof[0]='e';
	eof[1]='o';
	eof[2]='f';
	send(connected, &eof, 3, 0);	

	close(sock);
	return 0;
} 
