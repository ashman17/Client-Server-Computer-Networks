#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


// create connction
int create_connection(char* addr, int port) {
	int server_sockfd;
	int yes = 1;
	struct sockaddr_in server_addrinfo;

	// 1. SOCKET
    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("server: socket");
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
    // server_addrinfo.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0
    if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
        // printf("\nInvalid address/ Address not supported \n");
        close(server_sockfd);
        exit(1);
    }

	// 2. BIND
    if(bind(server_sockfd, (struct sockaddr*) &server_addrinfo, sizeof(server_addrinfo)) == -1){
        perror("server: bind");
        close(server_sockfd);
        exit(1);
    }
    
    // 3. LISTEN
    if(listen(server_sockfd, 1) == -1){
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

// Echo input from client
void echo_input(int socket_id) {
	int recv_count, send_count;
	char msg[1024];

	while (1) {
		memset(msg, 0, 1024);

		// 5. RECEIVE
		if((recv_count = recv(socket_id, msg, 1024, 0)) == -1){
			perror("recv");
			// close(server_sockfd);
			close(socket_id);
			exit(1);
		}

		if (strlen(msg) < 5) {
			strcpy(msg, "Error: Message length must be more than 5 characters");
		}

		// 6. SEND
		if((send_count = send(socket_id, msg, strlen(msg), 0)) == -1){
			perror("send");
			// close(server_sockfd);
			close(socket_id);
			exit(1);
		}
	}
	
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
    int client_id = client_connect(socket_id);
	echo_input(client_id);
    close(socket_id);


    return 0;    
}
