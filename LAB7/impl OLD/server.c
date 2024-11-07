#include "header.h"

#define PILANI 1
#define GOA 2
#define HYDERABAD 3

int socketID[NUM_CLIENTS+1] = {0};                      // 1 indexed
char pilaniDepts[NUM_CLIENTS+1][NAME_SIZE] = {0};       // 1 indexed
char goaDepts[NUM_CLIENTS+1][NAME_SIZE] = {0};          // 1 indexed
char hydDepts[NUM_CLIENTS+1][NAME_SIZE] = {0};          // 1 indexed


int pilaniSocketId, goaSocketId, hydSocketId;
int myCampus;

// create a socket and listen to connections
int listen_socket(char* addr, int port)
{
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

// accept incoming connection from a server or client
int accept_connection(int socket_id)
{
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

// create a socket and connect to a server
int connect_socket(char* addr, int port)
{
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
    if (inet_pton(AF_INET, addr, &server_addrinfo.sin_addr) <= 0) {
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

// receive input from client
void recv_data(int socket, struct packet* p)
{
	int recv_count;
    unsigned char* msg = malloc(1024);
    memset(msg, 0, 1024);
		
	// 5. RECEIVE
	if((recv_count = recv(socket, msg, 1024, 0)) == -1){
		perror("recv");
		close(socket);
		pthread_exit(0);
	}

    // copy the packet
    *p = *deserialize(msg);
    printPacket(p);
}

// send reply to client
void send_data(int socket, struct packet* p)
{
	int send_count;
    unsigned char* reply = serialize(p);

	// 6. SEND
	if((send_count = send(socket, (void*)reply, p->totalLength, 0)) == -1){
		perror("send");
		close(socket);
		pthread_exit(0);
	}
}

// worker thread to receive data from other servers and clients
void* recv_worker(void* arg)
{
    int socket = *(int*)(arg);

    while(1)
    {
        struct packet* p = malloc(sizeof(struct packet));
        recv_data(socket, p);

        // to ACK for the verification package between servers
        if (p->headerLength==6 && p->type==4 && p->ACK!=1)
        {
            if (p->srcCampus == PILANI)
                send_data(pilaniSocketId, makePacket(6, 0, 0, 0, 4, 1, myCampus, p->srcCampus, "Hi Pilani!"));

            if (p->srcCampus == GOA)
                send_data(goaSocketId, makePacket(6, 0, 0, 0, 4, 1, myCampus, p->srcCampus, "Hi Goa!"));

            if (p->srcCampus == HYDERABAD)
                send_data(hydSocketId, makePacket(6, 0, 0, 0, 4, 1, myCampus, p->srcCampus, "Hi Hyd!"));
        }

        // to update list of clients
        else if (p->headerLength==6 && p->type==2)
        {

            if (p->srcCampus == 1) memset(pilaniDepts, 0, sizeof(pilaniDepts));
            if (p->srcCampus == 2) memset(goaDepts, 0, sizeof(goaDepts));
            if (p->srcCampus == 3) memset(hydDepts, 0, sizeof(hydDepts));
            
            char* token = strtok(p->data, "=");
            char dept[NAME_SIZE];

            while (token)
            {
                strcpy(dept, token);
                token = strtok(NULL, ";");

                if (p->srcCampus == 1) strcpy(pilaniDepts[atoi(token)], dept);
                if (p->srcCampus == 2) strcpy(goaDepts[atoi(token)], dept);
                if (p->srcCampus == 3) strcpy(hydDepts[atoi(token)], dept);

                token = strtok(NULL,"=");
            }

            // send list to clients also
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (socketID[i] != -1)
                    send_data(socketID[i], makePacket(6, 0, 0, 0, 2, 0, p->srcCampus, myCampus, p->data));
            }
            
        }
    }
    pthread_exit(0);
}

int serverSetupPilani(char* addr, int goaPort, int hydPort)
{
    myCampus = PILANI;

    // we listen on one socket ID and when a connection is accepted
    // a new socket is created. Used same variable as listening socket
    // is no longer needed (i.e. listen to only one connection)
    goaSocketId = listen_socket(addr, goaPort);
    goaSocketId = accept_connection(goaSocketId);

    pthread_t goa_thread;
    if (pthread_create(&goa_thread, NULL, recv_worker, &goaSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    hydSocketId = listen_socket(addr, hydPort);
    hydSocketId = accept_connection(hydSocketId);

    pthread_t hyd_thread;
    if (pthread_create(&hyd_thread, NULL, recv_worker, &hydSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    send_data(goaSocketId, makePacket(6, 0, 0, 0, 4, 2, 1, 2, "Hello Goa! ~Pilani"));
    send_data(hydSocketId, makePacket(6, 0, 0, 0, 4, 2, 1, 3, "Hello Hyd! ~Pilani"));

    // join threads
    // if (pthread_join(goa_thread, NULL) != 0) 
    //     perror("pthread_join");
    // if (pthread_join(hyd_thread, NULL) != 0) 
    //     perror("pthread_join");

    return 1;
}

int serverSetupGoa(char* addr, int pilaniPort, int hydPort)
{
    myCampus = GOA;

    pilaniSocketId = connect_socket(addr, pilaniPort);

    pthread_t pilani_thread;
    if (pthread_create(&pilani_thread, NULL, recv_worker, &pilaniSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    hydSocketId = listen_socket(addr, hydPort);
    hydSocketId = accept_connection(hydSocketId);

    pthread_t hyd_thread;
    if (pthread_create(&hyd_thread, NULL, recv_worker, &hydSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    send_data(hydSocketId, makePacket(6, 0, 0, 0, 4, 2, 2, 3, "Hello Hyd! ~Goa"));

    // join threads
    // if (pthread_join(pilani_thread, NULL) != 0) 
    //     perror("pthread_join");
    // if (pthread_join(hyd_thread, NULL) != 0) 
    //     perror("pthread_join");

    return 1;
}

int serverSetupHyderabad(char* addr, int pilaniPort, int goaPort)
{
    myCampus = HYDERABAD;

    pilaniSocketId = connect_socket(addr, pilaniPort);
    
    pthread_t pilani_thread;
    if (pthread_create(&pilani_thread, NULL, recv_worker, &pilaniSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    goaSocketId = connect_socket(addr, goaPort);

    pthread_t goa_thread;
    if (pthread_create(&goa_thread, NULL, recv_worker, &goaSocketId) != 0)
    {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

    // if (pthread_join(pilani_thread, NULL) != 0) 
    //     perror("pthread_join");
    // if (pthread_join(goa_thread, NULL) != 0) 
    //     perror("pthread_join");

    return 1;
   
}

void* client_worker(void* arg)
{
    int socket = *(int*)arg;
    for (int i=1; i<=NUM_CLIENTS; i++) socketID[i] = -1; 

    while (1)
    {
        // 1. client connects
        int client_socket = accept_connection(socket);

        // 2. client sends packet with name
        struct packet* p = malloc(sizeof(struct packet));
        recv_data(client_socket, p);

        // 3. server finds a vacant ID for client
        int index = 1;
        p->data[strlen(p->data)-1] = '\0';
        while (index <= NUM_CLIENTS)
        {
            if (socketID[index] == -1)
            {
                socketID[index] = client_socket;
                if (myCampus == PILANI)
                    strcpy(pilaniDepts[index], p->data);
                if (myCampus == GOA)
                    strcpy(goaDepts[index], p->data);
                if (myCampus == HYDERABAD)
                    strcpy(hydDepts[index], p->data);
                break;
            }
            index++;
        }
        
        // 4. server sends ACK and LIST to client
        char list[1024];
        memset(list, 0, 1024);

        char pilaniCodes[1024];
        memset(pilaniCodes, 0, 1024);
        for (int i=1; i<=NUM_CLIENTS; i++)
        {
            if (strlen(pilaniDepts[i]) > 0)
                sprintf(pilaniCodes, "%s%s=%d;", pilaniCodes, pilaniDepts[i], i);
        }
        
        if (strlen(pilaniCodes)>0)
            sprintf(list, "P;%s", pilaniCodes);

        char goaCodes[1024];
        memset(goaCodes, 0, 1024);
        for (int i=1; i<=NUM_CLIENTS; i++)
        {
            if (strlen(goaDepts[i]) > 0)
                sprintf(goaCodes, "%s%s=%d;", goaCodes, goaDepts[i], i);
        }
        
        if (strlen(goaCodes)>0)
            sprintf(list, "%s|G;%s", list, goaCodes);
        
        char hydCodes[1024];
        memset(hydCodes, 0, 1024);
        for (int i=1; i<=NUM_CLIENTS; i++)
        {
            if (strlen(hydDepts[i]) > 0)
                sprintf(hydCodes, "%s%s=%d;", hydCodes, hydDepts[i], i);
        }
        
        if (strlen(hydCodes)>0)
            sprintf(list, "%s|H;%s", list, hydCodes);

        printf("HEHEHE\n"); fflush(stdout);
        send_data(client_socket, makePacket(5, 0, index, 0, 2, 1, 0, 0, list));

        // 5. server sends its updated list to other servers and departments
        if (myCampus == PILANI)
        {
            send_data(goaSocketId, makePacket(6, 0, 0, 0, 2, 0, 1, 2, pilaniCodes));
            send_data(hydSocketId, makePacket(6, 0, 0, 0, 2, 0, 1, 3, pilaniCodes));

            for (int i=1; i<NUM_CLIENTS; i++)
            {
                if (socketID[i] != -1)
                    send_data(socketID[i], makePacket(6, 0, 0, 0, 2, 0, 1, 1, pilaniCodes));
            }
        }

        else if (myCampus == GOA)
        {
            send_data(hydSocketId, makePacket(6, 0, 0, 0, 2, 0, 2, 3, goaCodes));
            send_data(pilaniSocketId, makePacket(6, 0, 0, 0, 2, 0, 2, 1, goaCodes));

            for (int i=1; i<NUM_CLIENTS; i++)
            {
                if (socketID[i] != -1)
                    send_data(socketID[i], makePacket(6, 0, 0, 0, 2, 0, 2, 2, goaCodes));
            }
        }

        else if (myCampus == HYDERABAD)
        {
            send_data(goaSocketId, makePacket(6, 0, 0, 0, 2, 0, 3, 2, hydCodes));
            send_data(pilaniSocketId, makePacket(6, 0, 0, 0, 2, 0, 3, 1, hydCodes));

            for (int i=1; i<NUM_CLIENTS; i++)
            {
                if (socketID[i] != -1)
                    send_data(socketID[i], makePacket(6, 0, 0, 0, 2, 0, 3, 3, hydCodes));
            }
        }
        
        // 6. create a thread to listen to messages of client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, recv_worker, &client_socket) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 6)
	{
		printf("Refer Qn for arguments\n");
		return -1;
	}
    char* addr = argv[1];
	int port1 = atoi(argv[2]);
    int port2 = atoi(argv[3]);
    int port3 = atoi(argv[4]);
    myCampus = atoi(argv[5]);


    if(myCampus == PILANI)
        serverSetupPilani(addr, port1, port2);
    else if(myCampus == GOA)
        serverSetupGoa(addr, port1, port2);
    else if(myCampus == HYDERABAD)
        serverSetupHyderabad(addr, port1, port2);
    else
    {
        printf("Invalid campus\n");
        return -1;
    }
    //server setup done

    //start listening for department connections and subsequently create threads to handle them

    int socket = listen_socket(addr, port3);

    pthread_t client_thread;
    if (pthread_create(&client_thread, NULL, client_worker, (void*)&socket) != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(client_thread, NULL) != 0) 
        perror("pthread_join");

    return 0;
}



