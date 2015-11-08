/* client of project MAN-KI-BAAT ver6
   creator : Vikas Patel (patel2vikas@gmail.com) */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>

struct arg_thread{
	int fd;
	char *client;
};



void *receive_t(void *data){
	char buffer[1024];

	char *name;
	int sock,temp;
	struct arg_thread *send_s;
	send_s =(struct arg_thread *) data;
	sock = send_s->fd;
	name = send_s->client;
	

	//printf("%s\n", "\nREADING SERVER ...");

	while(recv(sock,buffer,sizeof(buffer),0)){

		if(strncmp(buffer,name,sizeof(name))){	
			printf("T : ");
			timePrint();
			printf("# %s ",buffer);
			printf("\n");
		}

		memset(buffer,0,sizeof(buffer));
		printf("----------------------------------------------\n");
	}
}


void* send_t(void *data){
	char txbuffer[1024],message[1024];
	
	char *name;
	int sock,temp;
	struct arg_thread *send_s;
	send_s =(struct arg_thread *) data;
	sock = send_s->fd;
	name = send_s->client;
	
	//gets(buffer);
	//scanf("%[^\n]s",buffer);
	while(1){
		read(0,(void*)message,1024);
		printf("----------------------------------------------\n");
		write(1,"YOU : ",6);
		printf("%40s", message);	
		timePrint();
		printf("\n");

		memset(txbuffer,0,sizeof(txbuffer));
		strcpy(txbuffer,name);
		strcat(txbuffer, message);
		//printf("%s\n", txbuffer);
		
		temp = send (sock,txbuffer,sizeof(txbuffer),0);
		if (temp == 0)
			printf("%s\n", "SENDING ERROR");
	
		//printf("%s\n\n", "MSG SENDED");
		memset(txbuffer,0,sizeof(txbuffer));
		memset(message,0,sizeof(message));
		printf("----------------------------------------------\n");
	}
}


int main(int argc, char const *argv[])
{
	/* variables */
	int sock,temp;
	struct sockaddr_in server;
	struct hostent *hp;
	char buffer[1024];
	char name[20],txbuffer[2048];
	char ch;
	do{

		printf("%s\n", "Enter Your Name : ");
		scanf("%s",name);
		printf("CONFIRMATION : press y or n :");
		ch = getchar();
		ch = getchar();
	}while(ch != 'y');
	strcat(name," -> ");


	printf("%s\n", "INITIALIZING MAN-KI-BAAT ver1.0 ...");
	sleep(1);

	/* create socket */
	printf("%s\n", "CREATING SOCKET ...");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("SOCKET CREATE ERROR : ");
		exit(1);
	}
	printf("%s\n", "CREATED SOCKET ...");
	
	server.sin_family = AF_INET;


	/* getting IP from file*/
	char p[20];
	FILE *fp;
	fp = fopen("ip_client.txt","r");
	if(fp == NULL)
		printf("%s\n", "IP file not opened");

	fscanf(fp,"%s",p);
	printf("communating to : %s\n", p);
	fclose(fp);


	hp = gethostbyname(p);
	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);

	short int port;
    fp = fopen("port_client.txt","r");
    if(fp == NULL)
        printf("%s\n", "IP file not opened");

    fscanf(fp,"%hd",&port);
    printf("communating to port: %hd\n", port);
    fclose(fp);

	server.sin_port=htons(port);


	/* connect/request */
	printf("%s\n", "CONNECTING TO MAN-KI-BAAT SERVER ...");
	temp = connect(sock, (struct sockaddr *) &server, sizeof(server));
	if(temp != 0){
		perror("CONNECTION FAILED");
		exit(0);
	}
	printf("%s\n", "CONNECTED TO SERVER ...");

	pthread_t sendID,recvID;
	struct arg_thread send;
	send.fd = sock;
	send.client = name;

	pthread_create(&sendID,NULL,send_t,(void *)&send);
	pthread_create(&recvID,NULL,receive_t,(void *)&send);


	pthread_join(sendID,NULL);
	pthread_join(recvID,NULL);

	close(sock);
	return 0;
}		