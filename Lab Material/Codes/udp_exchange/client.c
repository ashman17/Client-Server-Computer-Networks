#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>


#define CLIENT_BUFF_SIZE 256

int status;
char buff[CLIENT_BUFF_SIZE];

int client(in_addr_t ip_addr, uint16_t in_port);
int log_con(const char* buff);
int log_err(const char* buff);

int main(int argc, char* argv[]) {
    if(argc<3){
        fprintf(stderr, "Insufficient arguments provided");
        return EXIT_FAILURE;
    }

    in_addr_t ip_addr = inet_addr(argv[1]);
    if(ip_addr == INADDR_NONE) {
        strncpy(buff, "Invalid IP address provided", CLIENT_BUFF_SIZE);
        exit(EXIT_FAILURE);
    }

    uint16_t in_port = htons(atoi(argv[2]));

    client(ip_addr, in_port);
    
    return EXIT_SUCCESS;
}

int log_err(const char* buff) {
    return fprintf(stderr, "[ERROR-SERVER] %s\n", buff);
}
int log_con(const char* buff) {
    return printf("[SERVER] %s\n", buff);
}

int client(in_addr_t ip_addr, uint16_t in_port) {
    // int sock = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1) {
        log_err("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in my_server;
    memset(&my_server, 0, sizeof(my_server));
    my_server.sin_family = AF_INET;
    my_server.sin_addr.s_addr = ip_addr;
    my_server.sin_port = in_port;

    printf("Please enter a message: ");
    scanf("%255[^\n]s", buff);          // Please note the length

    status = sendto(sock, buff, strlen(buff)+1, 0, (struct sockaddr*)&my_server, sizeof(my_server));        // Check return value for more error-proofing, note the length sent
    if(status == -1) {
        perror("[CLIENT] Unable to send");
        exit(EXIT_FAILURE);
    }

    
    if(close(sock) == -1) {
        perror("[CLIENT] Unable to clsoe the conenction");
    }

    return 0;

}