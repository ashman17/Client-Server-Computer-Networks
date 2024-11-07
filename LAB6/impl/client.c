#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

char username[50];
int socket_id;

//////////////////////////////////////////////////////////////////////////////

// Create a connection to the server
int create_connection(char* addr, int port) {
	int client_sockfd;
	struct sockaddr_in server_addrinfo;

	// 1. SOCKET
    if((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        perror("client: socket");
        exit(1);
    }

	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) 
	{
        perror("address");
        close(client_sockfd);
        exit(1);
    }

	// 2. CONNECT
    if(connect(client_sockfd, (struct sockaddr*)&server_addrinfo, sizeof(server_addrinfo)) == -1)
	{ 
        perror("connect");
        close(client_sockfd);
        exit(1);
    }

	return client_sockfd;

}

void send_data(int socket_id, char* reply) {
	int send_count;

	// 3. SEND
	if((send_count = send(socket_id, reply, strlen(reply), 0)) == -1)
	{
		perror("send");
		close(socket_id);
		exit(1);
	}
}

// Receive input from the server
void recv_data(int socket_id, char* msg) {
	int recv_count;
	
	// 4. RECEIVEs
	if((recv_count = recv(socket_id, msg, 1024, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

}

//////////////////////////////////////////////////////////////////////////////

void* recv_worker(void* arg) {

	char msg[1024];

	while (1) {
		memset(msg, 0, 1024);
		recv_data(socket_id, msg);

		if (!strcmp(msg, "EROR:PASSWORD REJECTED\n"))
		{
			printf("%s", msg);
			fflush(stdout);
			exit(0);
		}
		else if (!strncmp(msg, "SOFT|", 5))
		{
			char* token = strtok(msg, "|");
			token = strtok(NULL, "|");

			char filename[512] = "client-files/";
			strcat(filename, token);

			FILE* file = fopen(filename, "w");
			token = strtok(NULL, "|");

			while (token==NULL || strcmp(token, "EOFT"))
			{
				if (token!=NULL)
				{
					fprintf(file, "%s", token);
					fflush(file);
				}
				token = strtok(NULL, "|");
				if (token!=NULL && !strcmp(token, "EOFT"))
				{
					break;
				}
				memset(msg, 0, 1024);
				recv_data(socket_id, msg);
				token = strtok(msg, "|");
			}

			fclose(file);
		}
		else
		{
			printf("%s", msg);
			fflush(stdout);
		}

	}
}

void* send_worker(void* arg) {

	char usrauth[1024];
	fgets(usrauth, 1024, stdin);

	char hostname[1024];
    if (gethostname(hostname, 1024) != 0) {
        perror("Error getting hostname");
        exit(0);
    }
	
	struct hostent *host_entry;
	host_entry = gethostbyname(hostname);
	
	char *IPbuffer;
	IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

	char sendauth[1024];
	char* token = strtok(usrauth, "|");
	
	strcat(sendauth, token);
	strcat(sendauth, "@");
	strcat(sendauth, IPbuffer);
	
	token = strtok(NULL, "|");
	int normal = strcmp(token, "r\n");
	strcat(sendauth, "|");
	strcat(sendauth, token);

	send_data(socket_id, sendauth);

	if (!normal)
	{
		char password[1024];
		fgets(password, 1024, stdin);
		password[strlen(password)-1] = '\0';
		send_data(socket_id, password);
	}

	char cmd[1024];
	while(1) {
		memset(cmd, 0, 1024);
		fgets(cmd, 1024, stdin);
		send_data(socket_id, cmd);

		if(!strcmp(cmd, "EXIT\n")) {
			printf("CLIENT TERMINATED: EXITING......\n");
			fflush(stdout);
			close(socket_id);
			exit(0); // should make all threads exit...
		}
	}
}

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		perror("CLI");
		return -1;
	}
    
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);

    unsigned int port;
    port = atoi(argv[2]);

	socket_id = create_connection(addr, port);

	pthread_t recv_thread;
	if (pthread_create(&recv_thread, NULL, recv_worker, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

	pthread_t send_thread;
	if (pthread_create(&send_thread, NULL, send_worker, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

	if (pthread_join(recv_thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

	if (pthread_join(send_thread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

	// char msg[1024];
	// while (1) {
	// 	memset(msg, 0, 1024);
	// 	recv_data(socket_id, msg);
	// 	find_action(socket_id, msg);
	// }
}


