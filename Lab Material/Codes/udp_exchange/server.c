#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>


#define SERVER_BUFF_SIZE 256
#define SERVER_QUEUE_SIZE 1

int status;
char buff[SERVER_BUFF_SIZE];

int server(in_addr_t ip_addr, uint16_t in_port);
int log_con(const char* buff);
int log_err(const char* buff);

int main(int argc, char* argv[]) {
    if(argc<3){
        fprintf(stderr, "Insufficient arguments provided");
        return EXIT_FAILURE;
    }

    in_addr_t ip_addr = inet_addr(argv[1]);
    if(ip_addr == INADDR_NONE) {
        strncpy(buff, "Invalid IP address provided", SERVER_BUFF_SIZE);
        exit(EXIT_FAILURE);
    }

    uint16_t in_port = htons(atoi(argv[2]));

    server(ip_addr, in_port);

    exit(EXIT_SUCCESS);
}

int log_err(const char* buff) {
    return fprintf(stderr, "[ERROR-SERVER] %s\n", buff);
}

int log_con(const char* buff) {
    return printf("[SERVER] %s\n", buff);
}

int server(in_addr_t ip_addr, uint16_t in_port) {
    // int sock = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = ip_addr;
    server_addr.sin_port = in_port;

    status = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(status==-1) {
        perror("[SERVER] Binding error");
        exit(EXIT_FAILURE);
    }

    // status = recv(sock, buff, SERVER_BUFF_SIZE, 0);     //Fun stuff
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    status = recvfrom(sock, buff, SERVER_BUFF_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
    if(status==-1) {
        perror("[SERVER] Unable to receive");
        exit(EXIT_FAILURE);
    }

    log_con("Client says: ");
    log_con(buff);

    if(close(sock) == -1) {
        perror("[SERVER] Unable to clsoe the conenction");
    }

    return 0;

}