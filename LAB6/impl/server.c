#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
 
#define NUM_CLIENTS 1024
#define NUM_GROUPS 2048
 
struct client_info 
{
	int client_id;
	char username[256];
	int active; // 0 for no client, 1 for active client, -1 for deactivated client
	pthread_t* client_thread;
	int authority; // 0 for normal, 1 for root
};
 
struct group_info
{
	char groupname[64];
	struct client_info* users[NUM_CLIENTS];
	int num_users;
};
 
struct client_info* clients;
struct group_info* groups;
 
int server_id;
int group_count = 0;
 
 
sem_t* group_sem;
 
char password[1024];
 
////////////////////////////////////////////////////////////////////////////////////
 
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
void recv_data(struct client_info client, char* msg) {
	int recv_count;
	// memset(msg, 0, 1024);
		
	// 5. RECEIVE
	if((recv_count = recv(client.client_id, msg, 1024, 0)) == -1){
		perror("recv");
		close(client.client_id);
		exit(1);
	}
}
 
// send reply to client
void send_data(struct client_info client, char* reply) {
	int send_count;
 
	// 6. SEND
	if((send_count = send(client.client_id, reply, strlen(reply), 0)) == -1){
		perror("send");
		close(client.client_id);
		exit(1);
	}
}
 
////////////////////////////////////////////////////////////////////////////////////
 
void write_file(int code, char* msg, char* name1, char* name2)
{
	// code is 1 for MSGC, 2 for MCST, 3 for BCST
	// name1 is sender in MSGC and BCST, group name in MCST
 
	char filename[512];
 
	if (code==1)
	{
		for (int i=0; i<NUM_CLIENTS; i++)
		{
			if (!strcmp(clients[i].username, name1))
			{
				sprintf(filename, "server-files/01_%s-%s.txt", name1, name2);
				break;
			}
			else if (!strcmp(clients[i].username, name2))
			{
				sprintf(filename, "server-files/01_%s-%s.txt", name2, name1);
				break;
			}
			
		}
	}
 
	else if (code==2)
	{
		sprintf(filename, "server-files/02_%s.txt", name1);
	}
 
	else if (code==3)
	{
		strcpy(filename, "server-files/03_bcst.txt");
	}
 
	FILE* file = fopen(filename, "a");
	fprintf(file, "%s", msg);
	fclose(file);
}
 
void send_file(int code, struct client_info* client, char* name)
{	
	char filename[512];
	if (code==1)
	{
		char filename1[512];
		char filename2[512];
		
		sprintf(filename1, "server-files/01_%s-%s.txt", client->username, name);
		sprintf(filename2, "server-files/01_%s-%s.txt", name, client->username);
 
		write_file(1, "", client->username, name);
 
		if (access(filename1, F_OK) != -1)
		{
			strcpy(filename, filename1);
		}
		else if (access(filename2, F_OK) != -1)
		{
			strcpy(filename, filename2);
		}
	}
 
	else if (code==2)
	{
		// sem_wait(group_sem);
		int grpidx = -1;
		for (int i=0; i<group_count; i++)
		{
			if (!strcmp(groups[i].groupname, name))
			{
				grpidx = i;
				break;
			}
		}
 
		int found = 0;
		for (int i=0; i<groups[grpidx].num_users; i++)
		{
			if (!strcmp(groups[grpidx].users[i]->username, client->username))
			{
				write_file(2, "", name, NULL);
				sprintf(filename, "server-files/02_%s.txt", name);
				found = 1;
				break;
			}
		}
		
		if (!found)
		{
			send_data(*client, "EROR:UNAUTHORIZED\n");
			return;
		}
		// sem_post(group_sem);
	}
	
	else if (code==3)
	{	
		strcpy(filename, "server-files/03_bcst.txt");
		write_file(3, "", NULL, NULL);
	}
 
	FILE* file;
	file = fopen(filename, "r");
 
	char buffer[1024];
 
	char* token = strtok(filename, "/");
	token = strtok(NULL, "/");
 
	sprintf(buffer, "SOFT|%s|", token);
	send_data(*client, buffer);
	
	while (fgets(buffer, 1024, file))
	{
		send_data(*client, buffer);
	}
 
	send_data(*client, "|EOFT");
}
 
void client_auth(struct client_info* client)
{
	char data[100] = "";
	recv_data(*client, data);
 
	char* token = strtok(data, "|");
	strcpy(client->username, token);
 
	token = strtok(NULL, "|");
	client->authority = 0;
 
	if (!strncmp(token,"r", 1))
	{
		client->authority = 1;
		char pass[100] = "";
		recv_data(*client, pass);
 
		if (!strcmp(pass, password))
		{
			send_data(*client, "PASSWORD ACCEPTED\n");
		}
		else 
		{
			send_data(*client, "EROR:PASSWORD REJECTED\n");
			client->active = -1;
			close(client->client_id);
			pthread_exit(0);
		}
	}
 
	client->active = 1;
 
	char list[1024];
	memset(list, 0, 1024);
 
	strcpy(list, "LIST-");
 
	for (int i=0; i<NUM_CLIENTS; i++) 
	{
		if (clients[i].active == 1) 
		{
			strcat(list,clients[i].username);
			strcat(list, "|");
			if (clients[i].authority)
			{
				strcat(list, "r:");
			}
			else 
			{
				strcat(list, "n:");
			}
		}
	}
 
	int len = strlen(list);
	if (len>0) list[len-1] = '\n';
 
	for (int i=0; i<NUM_CLIENTS; i++) 
	{	
		if (clients[i].active==1)
			send_data(clients[i], list);
	}
}
 
void* recv_worker(void* arg) {
	struct client_info* client;
	client = (struct client_info*)arg;
 
	client_auth(client);
 
	char msg[1024];
	while (1) {
		memset(msg, 0, 1024);
		recv_data(*client, msg);
 
		if (!strcmp(msg, "EXIT\n")) 
		{
			client->active = -1;
			close(client->client_id);
 
			char list[1024];
			memset(list, 0, 1024);
 
			strcpy(list, "LIST-");
	
			for (int i=0; i<NUM_CLIENTS; i++) 
			{
				if (clients[i].active == 1) 
				{
					strcat(list,clients[i].username);
					strcat(list, "|");
					if (clients[i].authority)
					{
						strcat(list, "r:");
					}
					else 
					{
						strcat(list, "n:");
					}
				}
			}
	
			int len = strlen(list);
			if (len>0) list[len-1] = '\n';
 
			for (int i=0; i<NUM_CLIENTS; i++) 
			{	
				if ((client->client_id != clients[i].client_id) && clients[i].active==1)
					send_data(clients[i], list);
			}
 
			pthread_exit(0);
		}
 
		else if (!strcmp(msg, "LIST\n")) 
		{
			char list[1024];
			memset(list, 0, 1024);
 
			strcpy(list, "LIST-");
	
			for (int i=0; i<NUM_CLIENTS; i++) 
			{
				if (clients[i].active == 1) 
				{
					strcat(list,clients[i].username);
					strcat(list, "|");
					if (clients[i].authority)
					{
						strcat(list, "r:");
					}
					else 
					{
						strcat(list, "n:");
					}
				}
			}
	
			int len = strlen(list);
			if (len>0) list[len-1] = '\n';
	
			send_data(*client, list);
		}
 
		else if (!strncmp(msg, "MSGC:", 5)) 
		{
			char* token = strtok(msg, ":");
			token = strtok(NULL, ":");
			
			char* username;
			username = strdup(token);
 
			token = strtok(NULL, ":");
			char* msg;
 
			msg = strdup(token);
 
			char* reply;
			reply = strdup(client->username);
			strcat(reply, ":");
			strcat(reply, msg);
 
			int user_found = 0;
 
			for (int i=0; i<NUM_CLIENTS; i++) 
			{
				if (!strcmp(username, clients[i].username) && clients[i].active==1 )
				{
					send_data(clients[i], reply);
					write_file(1, reply, client->username, clients[i].username);
					user_found = 1;
				}
			}
 
			if (user_found==0) 
				send_data(*client, "USER NOT FOUND\n");
 
		}
 
		else if (!strncmp(msg, "BCST:", 5))
		{
			char* token = strtok(msg, ":");
			token = strtok(NULL, ":");
 
			char* reply;
			reply = strdup(client->username);
			strcat(reply, ":");
			strcat(reply, token);
 
			for (int i=0; i<NUM_CLIENTS; i++) 
			{	
				if ((client->client_id != clients[i].client_id) && clients[i].active==1)
					send_data(clients[i], reply);
			}
			write_file(3, reply, client->username, NULL);
		}
 
		else if (!strncmp(msg, "GRPS:", 5))
		{
			sem_wait(group_sem);
			char* token; char* names; char* groupname;
 
			token = strtok(msg, ":");
			token = strtok(NULL, ":");
			
			names = strdup(token);
			
			token = strtok(NULL,":");
			groupname = strdup(token);
			groupname[strlen(groupname)-1] = '\0'; // remove the \n
 
			token = strdup(names);
			token = strtok(token, ",");
 
			int valid = 1;
			while (token)
			{
				int found = 0;
				for (int i=0; i<NUM_CLIENTS; i++)
				{
					if (!strcmp(clients[i].username, token) && clients[i].active==1) 
					{
						found = 1;
						break;
					}
				}
				if (!found)
				{
					send_data(*client, "INVALID USERS LIST\n");
					valid = 0;
					break;
				}
				token = strtok(NULL, ",");
			}
 
			if (valid)
			{
				strcpy(groups[group_count].groupname, groupname);
 
				token = strtok(names, ",");
				int user_count = 0;
 
				while (token)
				{
					for (int i=0; i<NUM_CLIENTS; i++)
					{
						if (!strcmp(clients[i].username, token))
						{
							groups[group_count].users[user_count] = &clients[i];
						}
					}
					token = strtok(NULL, ",");
					user_count++;
				}
 
				groups[group_count].num_users = user_count;
 
				char data[100];
				sprintf(data, "GROUP %s CREATED\n", groups[group_count].groupname);
				send_data(*client, data);
 
				// create_files(2, groupname);
				group_count++;
			}
			sem_post(group_sem);
 
		}
		
		else if (!strncmp(msg, "MCST:", 5))
		{
			sem_wait(group_sem);
			char* token = strtok(msg, ":");
			token = strtok(NULL, ":");
 
			int grpidx = -1;
 
			for (int i=0; i<group_count; i++)
			{	
				if (!strcmp(token, groups[i].groupname))
					grpidx = i;
			}
 
			if (grpidx==-1)
			{
				char data[100];
				sprintf(data, "GROUP %s NOT FOUND\n", token);
				send_data(*client, data);
			}
 
			if (grpidx!=-1)
			{
				char* data;
				data = strdup(client->username);
				strcat(data, ":");
 
				token = strtok(NULL, ":");
				strcat(data, token);
 
				for (int i=0; i<groups[grpidx].num_users; i++)
				{
					struct client_info* recv_client = groups[grpidx].users[i];
					if (recv_client->active == 1)
						send_data(*recv_client, data);
				}
				write_file(2, data, groups[grpidx].groupname, NULL);
			}
			
			sem_post(group_sem);
		}
		
		else if (!strncmp(msg, "CAST:", 5))
		{
			if (client->authority != 1)
			{
				send_data(*client, "EROR:UNAUTHORIZED\n");
				continue;
			}
 
			char* token = strtok(msg, ":");
			token = strtok(NULL, ":");
 
			char* reply;
			reply = strdup(client->username);
			strcat(reply, ":");
			strcat(reply, token);
 
			for (int i=0; i<NUM_CLIENTS; i++) 
			{	
				if ((client->client_id != clients[i].client_id) && clients[i].active==1 && clients[i].authority==1)
				{
					send_data(clients[i], reply);
				}
					
			}
		}
 
		else if (!strncmp(msg, "HISF:", 5))
		{
			if (client->authority != 1)
			{
				send_data(*client, "EROR:UNAUTHORIZED\n");
				continue;
			}
			int t;
			sscanf(msg, "HISF:-t 0%d%*s", &t);
 
			if (t==1)
			{
				char username[256];
				sscanf(msg, "HISF:-t %*d|-n %s\n", username);
				send_file(1, client, username);
 
			}
 
			else if (t==2)
			{
				char groupname[256];
				sscanf(msg, "HISF:-t %*d|-n %s\n", groupname);
				send_file(2, client, groupname);
			}
 
			else if (t==3)
			{
				send_file(3, client, NULL);
			}
 
		}
		
		else
		{
			send_data(*client, "EROR:INVALID COMMAND\n");
		}
 
	}
 
}
 
void* connect_thread(void* args) {
 
	for (int count=0; count<NUM_CLIENTS; count++) {
		// char* username;
		int client_id = client_connect(server_id);
 
		clients[count].client_id = client_id;
		// send_data(clients[count], "NAME\n");
 
		// strncpy(clients[count].username, username, strlen(username));
		clients[count].client_thread = (pthread_t*) malloc (sizeof(pthread_t*));
 
		if (pthread_create(clients[count].client_thread, NULL, recv_worker, &clients[count]) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
 
	for (int i=0; i<NUM_CLIENTS; i++) {
		if (pthread_join(*clients[i].client_thread, NULL) != 0) {
			perror("pthread_join");
		}
	}
 
	pthread_exit(0);
}
 
int main(int argc, char *argv[])
{
    if (argc != 4)
	{
		printf("Use 3 cli arguments\n");
		return -1;
	}
	
	// extract the address and port from the command line arguments
	char addr[INET6_ADDRSTRLEN];
    unsigned int port;
 
	strcpy(addr, argv[1]);
    port = atoi(argv[2]);
 
	// NUM_CLIENTS = atoi(argv[3]);
	strcpy(password,argv[3]);
	
	clients = (struct client_info*) malloc(NUM_CLIENTS*sizeof(struct client_info));
	groups = (struct group_info*) malloc(NUM_GROUPS*sizeof(struct group_info));
 
	for (int i=0; i<NUM_CLIENTS; i++) clients[i].active = 0;
 
	server_id = create_connection(addr, port);
 
	// LINUX:
    group_sem = (sem_t*) malloc(sizeof(sem_t));
    sem_init(group_sem, 0, 1);
 
	// MAC
	// group_sem = sem_open("/group", O_CREAT, 0644, 1);
 
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, connect_thread, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
 
	// Wait for the thread to finish
    if (pthread_join(thread_id, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
	
	close(server_id);
 
	printf("SERVER TERMINATED: EXITING......\n");
	fflush(stdout);
 
    return 0;    
}