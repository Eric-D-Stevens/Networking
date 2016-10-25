#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

//These two functions check to see if a quit comand is given
int isQuit(char buffer[500]);
int isRecQuit(char buffer[500]);

int main() {
	int sockfd, portno, n; // socket, port #, and setup return
	struct sockaddr_in serveraddr; // server address
	struct hostent *server; 
	char hostname[50];
	char userHandle[50];
	char buf[BUFSIZE];
	char quitCmd[510];

	system("clear");

	// host name
	printf("Please Enter The Host Name: ");
	gets(hostname);
	//sprintf(hostname,"localhost");


	//Port Number
	char portstr[6];
	printf("Please Enter the Port Number: ");
	gets(portstr);
	portno=atoi(portstr);	
	//portno=9999;
	
	// Get User Handle
	printf("Enter Desired User Handle: ");
	//scanf("%s", userHandle);
	gets(userHandle);
	//sprintf(userHandle, "FUKWORK");

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname);

	/* build the server's Internet address */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
			(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	/* connect: create a connection with the server */
	connect(sockfd, &serveraddr, sizeof(serveraddr)); 
	
	
	printf("\n\n--------------CONNECTION ESTABLISHED--------------\n\n");
	
	while(1){
		char temp[510];
		temp[0] = '\0';
		/* get message line from the user */
		printf("%s >>> ", userHandle);
		gets(temp);
		sprintf(buf, userHandle);
		strcat(buf,">>> ");
		strcat(buf, temp);
		strcat(buf, "\0");

		/* send the message line to the server */
		n = write(sockfd, buf, strlen(buf));
		
		if(isQuit(temp)==1){
			system("clear");
			printf("\n\n QUIT CONNECTION - CLOSEING SOCKET\n\n");
			close(sockfd);
			break;
		}

		printf("\n");

		/* print the server's reply */
		bzero(buf, BUFSIZE);
		n = read(sockfd, buf, BUFSIZE);
		printf("%s\n\n", buf);
		if(isRecQuit(buf)==1){
			system("clear");
			printf("\n\n QUIT CONNECTION - CLOSEING SOCKET\n\n");
			close(sockfd);
			break;
		}

	}

	return 0;

}


int isQuit(char buffer[500]){

	int toReturn = 0;

	char quitCMD[5];
	quitCMD[0]='\\';
	quitCMD[1]='q';
	quitCMD[2]='u';
	quitCMD[3]='i';
	quitCMD[4]='t';

	int i;
	for(i=0; i<5; i++){
		if(buffer[i] != quitCMD[i]){return 0;}
	}


	return 1;

	


}



int isRecQuit(char buffer[500]){

	int toReturn = 0;

	char quitCMD[19];
	

	quitCMD[0]='C';
	quitCMD[1]='h';
	quitCMD[2]='a';
	quitCMD[3]='t';
	quitCMD[4]='S';
	quitCMD[5]='e';
	quitCMD[6]='r';
	quitCMD[7]='v';
	quitCMD[8]='e';
	quitCMD[9]=' ';
	quitCMD[10]='>';
	quitCMD[11]='>';
	quitCMD[12]='>';
	quitCMD[13]=' ';
	quitCMD[14]='\\';
	quitCMD[15]='q';
	quitCMD[16]='u';
	quitCMD[17]='i';
	quitCMD[18]='t';
	int i;
	for(i=0; i<19; i++){
		if(buffer[i] != quitCMD[i]){return 0;}
	}


	return 1;

	


}





