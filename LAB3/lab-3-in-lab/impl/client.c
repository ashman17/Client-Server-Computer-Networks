#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

FILE* file;

void write_file(char* str, int level) {
	char temp[1024];
	file = fopen("client_file.txt", "a");
	
	if (level==0) {
		sprintf(temp, "%s: %s", "CLIENT", str);
	} else if (level==1) {
		sprintf(temp, "%s: %s\n", "SERVER", str);
	}

	fputs(temp, file);
	fclose(file);
}

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
        // printf("\nInvalid address/ Address not supported \n");
        close(client_sockfd);
        exit(1);
    }

	// 2. CONNECT
    if(connect(client_sockfd, (struct sockaddr*)&server_addrinfo, sizeof(server_addrinfo)) == -1){ // client connects if server port has started listen()ing and queue is non-full; however server connects to client only when it accept()s
        printf("Could not find server");
        close(client_sockfd);
        exit(1);
    }

	file = fopen("client_file.txt", "w");
	fclose(file);

	return client_sockfd;

}

void send_data(int socket_id) {
	int send_count;
	char msg[1024];
	fgets(msg, 1024, stdin);

	if(strcmp(msg, "EXIT\n") == 0){
		// 5. CLOSE
		printf("Client exited successfully");
		close(socket_id);
		exit(0);
	}

	// 3. SEND
	if((send_count = send(socket_id, msg, strlen(msg)-1, 0)) == -1){
		perror("send");
		close(socket_id);
		exit(1);
	}

	write_file(msg, 0);
}

// Receive input from the server
void recv_data(int socket_id) {
	int recv_count;
	char reply[1024];
	memset(reply, 0, 1024);
	
	// 4. RECEIVE
	if((recv_count = recv(socket_id, reply, 1024, 0)) == -1){
		perror("recv");
		exit(1);
	}

	write_file(reply, 1);
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

	while (1)
    {
        send_data(socket_id);
        recv_data(socket_id);
    }
}


