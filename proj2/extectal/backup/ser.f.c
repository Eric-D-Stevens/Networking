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


int main(int argc, char * argv[])
{

	int servprt = atoi(argv[1]);
	int sock, connected, bytes_recieved , true = 1;  
	char send_data [1024] , recv_data[1024];       

	struct sockaddr_in server_addr,client_addr;    
	int sin_size;

	while(1)
	{  
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			exit(1);
		}
	
		if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
			perror("Setsockopt");
			exit(1);
		}
	
		server_addr.sin_family = AF_INET;         
		server_addr.sin_port = htons(servprt);     
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
	
		printf("\n\nSERVER OPEN ON PORT %d\n", servprt);
		fflush(stdout);



		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
		struct hostnet *he;	
		
		char hostName[100];
		bytes_recieved = recv(connected,hostName,100,0);
		hostName[bytes_recieved] = '\0';		


		printf("CONNECTION FROM %s\n", hostName);
		fflush(stdout);
	
		char dataport[10];
		bytes_recieved = recv(connected,dataport,10,0);
		dataport[bytes_recieved] = '\0';
		int dataPortNum = atoi(dataport);	
		while (1)
		{
			if(strcmp(&dataport, "noOW") == 0)
			{	
				printf("%s");
				close(sock);
				break;
			}

			
			char ch = 'o';
			send(connected, &ch, 1, 0);  
			
			bytes_recieved = recv(connected,recv_data,1024,0);
			recv_data[bytes_recieved] = '\0';
			


			if (strcmp(recv_data , "-l") == 0)
			{
				printf("LIST DIRECTORY REQUESTED ON PORT %d\n", dataPortNum);
				printf("SENDING DIRECTORY CONTENTS TO %s:%d\n",hostName, dataPortNum);
				char command[50];
				strcpy(command, "ls > temp1");
				system(command); 
				SendData(connected, dataPortNum);
				strcpy(command, "rm -f temp1");
				system(command); 
				close(sock);
				break;
			}


			else  if (recv_data[0] == '-' && recv_data[1] == 'g')
			{
				fflush(stdout);
				char data[100];
				sprintf(data, "%s", recv_data);
				char *filename;
				filename = strtok(data, " ");
				filename = strtok(NULL, " ");
				if(access(filename, F_OK) == -1)
				{
					printf("\nFile %s does not exist\n", filename);
					fflush(stdout);
					char nof[7];
					sprintf(nof,"no$$ff");
					send(connected, &nof, 7, 0);
					close(sock);
					break;
				}

				printf("FILE \"%s\" REQUESTED ON PORT %d\n", filename, dataPortNum);
				fflush(stdout);	
				
				char command[50];
				 
				printf("SENDING \"%s\" ON PORT %d\n", filename, dataPortNum);
				fflush(stdout);	

				sprintf(&command, "cat %s > temp1", filename);
				system(command); 


				SendData(connected,dataPortNum);
				strcpy(command, "rm -f temp1");
				system(command); 
				close(sock);
				break;
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




int SendData(int connected, int dataPort){

	int tempcon = connected;	


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
	server_addr.sin_port = htons(dataPort);     
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

			//	printf("\nIN GGGGGG"); fflush(stdout);


	char redmsg[20];
	strcpy(redmsg," ");
	send(tempcon, &redmsg, 1, 0);  

	sin_size = sizeof(struct sockaddr_in);

	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);


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
