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

#define NUM_CLIENTS 6
#define NAME_SIZE 32

struct packet {
    int version;
    int headerLength;
    int totalLength;
    int srcDept;
    int destDept;
    int checkSum;
    int hops;
    int type;
    int ACK;
    int srcCampus;
    int destCampus;
    char data[1024];
};

//You can add the implementations of functions in helper.c to avoid code cluttering

void printPacket(struct packet *p);
unsigned char* serialize( struct packet* p);
struct packet *deserialize(unsigned char* buffer);

struct packet *generatePacket(int version, int headerLength, int totalLength, 
                              int srcDept, int destDept, int checkSum, int hops, 
                              int type, int ACK, int srcCampus, int destCampus, char* data);
struct packet* generateUnicastPacket(char* input);
struct packet* generateBroadcastPacket(char* input);
struct packet* generateControlPacket(char* input);

// new functions
struct packet *makePacket(int headerLength, int srcDept, int destDept, int hops,
                        int type, int ACK, int srcCampus, int destCampus, char* data);