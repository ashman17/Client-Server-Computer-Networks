#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char username[50];

int create_connection(char* addr, int port);
void send_data(int socket_id, char* reply);
void recv_data(int socket_id, char* msg);

// Create a connection to the server
int create_connection(char* addr, int port) {
	int client_sockfd;
	struct sockaddr_in server_addrinfo;

	// 1. SOCKET
    if((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("client: socket");
        exit(1);
    }

	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
        perror("address");
        close(client_sockfd);
        exit(1);
    }

	// 2. CONNECT
    if(connect(client_sockfd, (struct sockaddr*)&server_addrinfo, sizeof(server_addrinfo)) == -1){ // client connects if server port has started listen()ing and queue is non-full; however server connects to client only when it accept()s
        perror("connect");
        close(client_sockfd);
        exit(1);
    }

	return client_sockfd;

}

void send_data(int socket_id, char* reply) {
	int send_count;

	// 3. SEND
	if((send_count = send(socket_id, reply, strlen(reply), 0)) == -1){
		perror("send");
		close(socket_id);
		exit(1);
	}
}

// Receive input from the server
void recv_data(int socket_id, char* msg) {
	int recv_count;
	
	// 4. RECEIVEs
	if((recv_count = recv(socket_id, msg, 1024, 0)) == -1){
		perror("recv");
		exit(1);
	}

}

void find_action(int socket_id, char* msg) {

	if (!strcmp(msg, "NAME\n")) {
		printf("INITIALIZING......\n");
		fflush(stdout);
		send_data(socket_id, username);
	} 
	else if (!strcmp(msg, "POLL\n")) {
		printf("ENTER CMD: ");
		fflush(stdout);

		char cmd[1024];
		fgets(cmd, 50, stdin);
	
		send_data(socket_id, cmd);

		if (!strcmp(cmd, "NOOP\n")) {
			// no operation
		}
		else if(!strcmp(cmd, "EXIT\n")) {
			printf("CLIENT TERMINATED: EXITING......\n");
			fflush(stdout);
			close(socket_id);
			exit(0);
		}
		else if (!strcmp(cmd, "LIST\n")) {
			char list[1024];
			memset(list, 0, 1024);
			recv_data(socket_id, list);

			// char* pos = strstr(list, "POLL\n"); 
			// if (pos) *pos = '\0';s
			char* names = strtok(list, "\n");
			
			char new_list[1024];
			memset(new_list, 0, 1024);

			char* name = strtok(names, ":");
			char temp[5];
			int count = 1;

			while (name) {
				sprintf(temp, "%d. ", count);
				strcat(new_list, temp);
				strcat(new_list, name);
				name = strtok(NULL, ":");
				if (name) strcat(new_list, "\n");
				count++;
			}

			printf("%s\n", new_list);
			fflush(stdout);

			names = strtok(NULL, "\n");
			if (names && !strcmp(names, "POLL\n")) 
				find_action(socket_id, "POLL\n");

			// if (pos) find_action(socket_id, "POLL\n");
		} else {
			// no action needed from client
		}

	}
}

int main(int argc, char *argv[])
{
    if (argc != 4)
	{
		perror("CLI");
		return -1;
	}
    
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
	strcpy(addr, argv[1]);

    unsigned int port;
    port = atoi(argv[2]);

	strcpy(username, argv[3]);
	strcat(username, "\n");

	int socket_id = create_connection(addr, port);

	char msg[1024];
	while (1) {
		memset(msg, 0, 1024);
		recv_data(socket_id, msg);
		find_action(socket_id, msg);
	}
}


