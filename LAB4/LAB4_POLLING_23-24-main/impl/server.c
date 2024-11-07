#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define NUM_CLIENTS 3
 
struct client_info {
	int client_id;
	char username[50];
	int active;
};
 
struct client_info clients[NUM_CLIENTS];
 
// create connction
int create_connection(char* addr, int port) {
	int server_sockfd;
	int yes = 1;
	struct sockaddr_in server_addrinfo;
 
	// 1. SOCKET
    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
 
	// SockOptions
    if(setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("setsockopt");
        close(server_sockfd);
        exit(1);
    }
 
	server_addrinfo.sin_family = AF_INET;
    server_addrinfo.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
        perror("addr");
        close(server_sockfd);
        exit(1);
    }
 
	// 2. BIND
    if(bind(server_sockfd, (struct sockaddr*) &server_addrinfo, sizeof(server_addrinfo)) == -1){
        perror("bind");
        close(server_sockfd);
        exit(1);
    }
    
    // 3. LISTEN
    if(listen(server_sockfd, NUM_CLIENTS) == -1){
        perror("listen");
        close(server_sockfd);
        exit(1);
    }
 
	return server_sockfd;
}
 
// Accept incoming connections
int client_connect(int socket_id) {
	int new_server_sockfd;
	socklen_t sin_size;
	struct sockaddr_in client_addrinfo;
 
	sin_size = sizeof(client_addrinfo);
 
    // 4. ACCEPT
    new_server_sockfd = accept(socket_id, (struct sockaddr*) &client_addrinfo, &sin_size);
    if(new_server_sockfd == -1){
        perror("accept");
        close(socket_id);
        exit(1);
    }
 
	return new_server_sockfd;
}
 
// receive input from client
void recv_data(int socket_id, char* msg) {
	int recv_count;
	// memset(msg, 0, 1024);
		
	// 5. RECEIVE
	if((recv_count = recv(socket_id, msg, 1024, 0)) == -1){
		perror("recv");
		close(socket_id);
	}
}
 
// send reply to client
void send_data(int socket_id, char* reply) {
	int send_count;
 
	// 6. SEND
	if((send_count = send(socket_id, reply, strlen(reply), 0)) == -1){
		perror("send");
		close(socket_id);
		exit(1);
	}
}
 
void find_action(int num, char* msg) {
 
	if (!strcmp(msg, "NOOP\n")) {
		// no action
	}
	else if (!strcmp(msg, "EXIT\n")) {
		clients[num].active = 0;
		close(clients[num].client_id);
	}
	else if (!strncmp(msg, "MESG:", 5)) {
		char* token = strtok(msg, ":");
		token = strtok(NULL, ":");
		printf("%s:%s", clients[num].username, token);
		fflush(stdout);
	}
	else if (!strcmp(msg, "LIST\n")) {
		char list[1024];
		memset(list, 0, 1024);
 
		for (int i=0; i<NUM_CLIENTS; i++) {
			if (clients[i].active) {
				strcat(list,clients[i].username);
				strcat(list,":");
			}
		}
 
		int len = strlen(list);
		if (len>0) list[len-1] = '\n';
 
		send_data(clients[num].client_id, list);
	} else {
		printf("INVALID CMD\n");
		fflush(stdout);
	}
}
 
int poll() {
	int flag = 0;
	char msg[1024];
	
	for (int i=0; i<NUM_CLIENTS; i++) {
		memset(msg, 0, 1024);
		if (clients[i].active) {
			flag = 1;
			send_data(clients[i].client_id, "POLL\n");
			recv_data(clients[i].client_id, msg);
			find_action(i, msg);
		}
	}
 
	return flag;
}
 
int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		printf("Use 2 cli arguments\n");
		return -1;
	}
	
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
    unsigned int port;
 
	strcpy(addr, argv[1]);
    port = atoi(argv[2]);
 
	int socket_id = create_connection(addr, port);
 
	char username[50];
	for (int count=0; count<NUM_CLIENTS; count++) {
		int client_id = client_connect(socket_id);
		send_data(client_id, "NAME\n");
		recv_data(client_id, username); 
		clients[count].client_id = client_id;
		clients[count].active = 1;
		strncpy(clients[count].username, username, strlen(username)-1);
	}
 
	while (poll()) {}
	
	close(socket_id);
 
	printf("SERVER TERMINATED: EXITING......\n");
	fflush(stdout);
 
    return 0;    
}