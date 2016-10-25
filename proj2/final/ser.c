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
	// SETUP
	int servprt = atoi(argv[1]);				// get port on from command line
	int sock, connected, bytes_recieved , true = 1;  	// various variables
	char send_data [1024] , recv_data[1024];       		// buffers

	struct sockaddr_in server_addr,client_addr;    		// ser/cli socket variables
	int sin_size;

	// LOOP FOR REPEAT LISTENING ON PORT <SERVER PORT>
	while(1)
	{  	
		// initalize socket
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			exit(1);
		}
		// set options
		if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
			perror("Setsockopt");
			exit(1);
		}
		// set port up
		server_addr.sin_family = AF_INET;         
		server_addr.sin_port = htons(servprt);     
		server_addr.sin_addr.s_addr = INADDR_ANY; 
		bzero(&(server_addr.sin_zero),8); 
		
		//BIND
		if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
				== -1) {
			perror("Unable to bind");
			exit(1);
		}
		// Listen on socket
		if (listen(sock, 5) == -1) {
			perror("Listen");
			exit(1);
		}
	
		printf("\n\nSERVER OPEN ON PORT %d\n", servprt);
		fflush(stdout);


		// Accept incoming connetion request
		sin_size = sizeof(struct sockaddr_in);
		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
		struct hostnet *he;	
		
		// First recieve client name
		char hostName[100];					// buffer
		bytes_recieved = recv(connected,hostName,100,0);	// receive
		hostName[bytes_recieved] = '\0';			// terminal char

		// Print back host name
		printf("CONNECTION FROM %s\n", hostName);
		fflush(stdout);
	
		// Next receive desired dataport form client
		char dataport[10];					// buffer
		bytes_recieved = recv(connected,dataport,10,0);		// receive
		dataport[bytes_recieved] = '\0';			// terminal char
		int dataPortNum = atoi(dataport);			// convert to int
		
		// Internal loop
		while (1)
		{
			// Process write option if file exists in client directory
			if(strcmp(&dataport, "noOW") == 0)		// client sends "noOW"
			{	
				printf("\nCLIENT REFUSED OVERWRITE\n");
				close(sock);
				break;
			}

			
			char ch = 'o';					// random char
			send(connected, &ch, 1, 0);  			// sent for timing
			

			// RETRIEVE MAIN COMMAND PARAMETERS
			bytes_recieved = recv(connected,recv_data,1024,0); 	//retreive
			recv_data[bytes_recieved] = '\0';			//terminate
			

			// if the -l comand was received
			if (strcmp(recv_data , "-l") == 0)
			{
				printf("LIST DIRECTORY REQUESTED ON PORT %d\n", dataPortNum);
				printf("SENDING DIRECTORY CONTENTS TO %s:%d\n",hostName, dataPortNum);
				char command[50];			// system buffer
				strcpy(command, "ls > temp1");		// command string
				system(command); 			// cat contents to temp file
				SendData(connected, dataPortNum);	// CALL THE FILE SENDING FUNCTION	
				strcpy(command, "rm -f temp1");		// command string
				system(command); 			// remove temp file
				close(sock);				// close connection
				break;					// break inner loop
			}

			// if the -g command was received
			else  if (recv_data[0] == '-' && recv_data[1] == 'g')
			{
				fflush(stdout);			
				char data[100];				// data buffer
				sprintf(data, "%s", recv_data);		// copy received data into buffer
				char *filename;				// declare filename string
				filename = strtok(data, " ");		// token out filename
				filename = strtok(NULL, " ");		// second token is the file name
				if(access(filename, F_OK) == -1)	// see if this file exists in server directory
				{
					printf("\nFile %s does not exist\n", filename);	
					fflush(stdout);	
					char nof[7];			// buffer for 'does not exist' message
					sprintf(nof,"no$$ff");		// send 'does not exist' message
					send(connected, &nof, 7, 0);	
					close(sock);			// close connecton
					break;				// break
				}
				
				// File exists, prepare to ship
				printf("FILE \"%s\" REQUESTED ON PORT %d\n", filename, dataPortNum);
				fflush(stdout);	
				
				char command[50]; 	//system command buffer
				 
				printf("SENDING \"%s\" ON PORT %d\n", filename, dataPortNum);
				fflush(stdout);	

				sprintf(&command, "cat %s > temp1", filename);	// prepare system command
				system(command); 				// create tempfile with file to send

				// Call data file transfer function
				SendData(connected,dataPortNum);
				strcpy(command, "rm -f temp1");			// remove temp file
				system(command); 				
				close(sock);					// terminate connection
				break;						// break out of inner loop
			}


			else
			{ 
				break;
			}
		}
	}       
			
	close(sock);
	return 0;
}



// Function is responsible for sending the data contained in the 
// prevously generated temp file 'temp1'. Also it is responsible
// for generating the data connection that will be used to transfer
// the file
int SendData(int connected, int dataPort){
// connectd is the socket number of the origonial connection
// data port is the port that the data will be dransfered through
	

	int tempcon = connected;			// temporary connection int
	int sock, bytes_recieved , true = 1;  		// variables for data connections
	char send_data [1024] , recv_data[1024];       	// buffers for data port

	struct sockaddr_in server_addr,client_addr;    	// data port structure
	int sin_size;

// INITALIZE DATA PORT

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
	
	// bind data port
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}
	
	// listen on data port
	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}


	// timing message to connection
	char redmsg[20];
	strcpy(redmsg," ");
	send(tempcon, &redmsg, 1, 0);  

	sin_size = sizeof(struct sockaddr_in);
	
	// accept connection to data port
	connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

	// File pointer to open temp file
	FILE *fp;
	char *filename = "temp1";
	int c;
	int n = 0;

	fp = fopen(filename,"r");		// open temp file for reading
	if(fp == NULL){
		perror("fail to open temp");
	}
	
	// loop grabs one character at a time until it hits end of flile			
	do{
		c = fgetc(fp);				// get next char
		if(feof(fp)){break;}			// break if eof
		char ch = (char) c;			// typecast
		send(connected, &ch, 1, 0);  		// send char
	}while(1);					
	fclose(fp);					// close
	char eof[3];					// eof signal to client
	eof[0]='e';
	eof[1]='o';
	eof[2]='f';
	send(connected, &eof, 3, 0);			// send eof signal to client

	close(sock);					// close data connection
	return 0;					// return
} 
