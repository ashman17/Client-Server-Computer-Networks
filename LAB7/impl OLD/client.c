#include "header.h"

int socketID;
int myCampusID;
char deptname[NAME_SIZE];
int deptID;

char pilaniDepts[NUM_CLIENTS+1][NAME_SIZE];
char goaDepts[NUM_CLIENTS+1][NAME_SIZE];
char hydDepts[NUM_CLIENTS+1][NAME_SIZE];

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

//input in the below functions are the strings obtained from the 
//user through stdin in its entirety. 

struct packet* generateUnicastPacket(char* input)
{
	//code fore generating Unicast packets
	//Use for inputs starting from 1 and 2

    struct packet* p;

    if (!strncmp(input, "1", 1))
    {
        input += 2; // remove the 1.

        char* token = strtok(input, ":");
        char* destCampus = strdup(token);

        token = strtok(NULL, ":");
        char* destDept = strdup(token);

        token = strtok(NULL, ":");
        char* payload = strdup(token);

        int destCampusID = 0;
        int destDeptID = 0;

        if (!strcmp(destCampus, "P"))
        {
            destCampusID = 1;
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                printf("DEPT: %s\n", pilaniDepts[i]); fflush(stdout);
                if (!strcmp(pilaniDepts[i], destDept))
                    destDeptID = i;
            }
        }
        else if (!strcmp(destCampus, "G"))
        {
            destCampusID = 2;
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (!strcmp(goaDepts[i], destDept))
                    destDeptID = i;
            }
        }
            
        else if (!strcmp(destCampus, "H"))
        {
            destCampusID = 3;
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (!strcmp(hydDepts[i], destDept))
                    destDeptID = i;
            }
        }

        p = makePacket(6, deptID, destDeptID, 0, 0, 0, myCampusID, destCampusID, payload);
    }

    else if (!strncmp(input, "2", 1))
    {
        input += 2; // remove the 2.

        char* token = strtok(input, ":");
        char* destDept = strdup(token);

        token = strtok(NULL, ":");
        char* payload = strdup(token);

        int destDeptID = 0;

        if (myCampusID == 1)
        {
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (!strcmp(pilaniDepts[i], destDept))
                    destDeptID = i;
            }
        }

        else if (myCampusID == 2)
        {
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (!strcmp(goaDepts[i], destDept))
                    destDeptID = i;
            }
        }

        else if (myCampusID == 3)
        {
            for (int i=1; i<=NUM_CLIENTS; i++)
            {
                if (!strcmp(hydDepts[i], destDept))
                    destDeptID = i;
            }
        }

        p = makePacket(5, deptID, destDeptID, 0, 0, 0, 0, 0, payload);
    }
    
    printPacket(p);
    return p;
}

struct packet* generateBroadcastPacket(char* input)
{
	//code for generating broadcast packets
	//Use for unputs starting with 3 aand 4

    struct packet* p;
    return p;
}

struct packet* generateControlPacket(char* input)
{
	//code for generating control packets
	//Use for inputs starting with 5
    
    struct packet* p;
    p = makePacket(5, deptID, 0, 0, 1, 0, 0, 0, input);
    return p;
}

void* recv_worker(void* arg) {

	while (1) 
    {

	}
}

void* send_worker(void* arg) {

	while(1) 
    {
        printf("ENTER CMD: ");
        fflush(stdout);
        
        char cmd[1024];
		fgets(cmd, 50, stdin);

        struct packet* p = malloc(sizeof(struct packet));

        if (!strncmp(cmd,"1",1) || !strncmp(cmd,"2",1))
            p = generateUnicastPacket(cmd);
        else if (!strncmp(cmd,"3",1) || !strncmp(cmd,"4",1))
            p = generateBroadcastPacket(cmd);
        else if (!strncmp(cmd,"5",1))
            p = generateControlPacket(cmd);
        
        
	}
}

void setCodes(int campusID, char* input)
{
    char* token = strtok(input, "=");
    char dept[NAME_SIZE];

    while (token)
    {
        strcpy(dept, token);
        token = strtok(NULL, ";");

        if (campusID == 1) strcpy(pilaniDepts[atoi(token)], dept);
        if (campusID == 2) strcpy(goaDepts[atoi(token)], dept);
        if (campusID == 3) strcpy(hydDepts[atoi(token)], dept);

        token = strtok(NULL,"=");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
	{
		printf("Refer Qn for arguments\n");
        fflush(stdout);
		exit(1);
	}
    
	// extract the address and port from the command line arguments
	char* addr = argv[1];
	int port = atoi(argv[2]);

    myCampusID = atoi(argv[3]);

    char* department = argv[4];
    strcat(department, "\n");
    
    // 1. connect to server
	socketID = connect_socket(addr, port);

    // 2. send name to server
    send_data(socketID, makePacket(5, 111, 000, 0, 0, 0, 0, 0, department));

    // 3. receive ACK (with ID) and the list
    struct packet* p = malloc(sizeof(struct packet));
    recv_data(socketID, p);
    deptID = p->destDept;

    // 4. populate the IDs using the list
    char* list = strdup(p->data);

    char* token = strtok(list, "|");
    char* campus;
    
    while (token)
    {
        campus = strdup(token);
        
        if (!strncmp(campus, "P;", 2)) setCodes(1, campus+2);
        else if (!strncmp(campus, "G;", 2)) setCodes(2, campus+2);
        else if (!strncmp(campus, "H;", 2)) setCodes(3, campus+2);
        else perror("invalid campus");
        
        token = strtok(NULL, "|");
    }

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

    if (pthread_join(recv_thread, NULL) != 0) 
        perror("pthread_join");
    if (pthread_join(send_thread, NULL) != 0) 
        perror("pthread_join");

    return 0;
}