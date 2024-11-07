#include "header.h"

void printPacket(struct packet *p){  //always follow this format for printing the packet
    printf("{\n");
    printf("    Version: %d\n", p->version);
    printf("    Header Length: %d\n", p->headerLength);
    printf("    Total Length: %d\n", p->totalLength);
    printf("    Source Department: %d\n", p->srcDept);
    printf("    Destination Department: %d\n", p->destDept);
    printf("    CheckSum: %d\n", p->checkSum);
    printf("    Hops: %d\n", p->hops);
    printf("    Type: %d\n", p->type);
    printf("    ACK: %d\n", p->ACK);
    if(p->headerLength == 6){
        printf("    Source Campus: %d\n", p->srcCampus);
        printf("    Destination Campus: %d\n", p->destCampus);
    }
    printf("    Data: %s\n", p->data);
    printf("}\n");
    fflush(stdout);
}

unsigned char* serialize(struct packet* p){
    unsigned char *buffer;
    buffer = malloc(p->totalLength);
    memset(buffer, 0, p->totalLength);

    buffer[0] = (unsigned char)((p->version << 4) | p->headerLength);
    buffer[1] = (unsigned char)(p->totalLength);
    buffer[2] = (unsigned char)((p->srcDept << 5 | p->destDept << 2 | ((p->checkSum & 0b1100000000) >> 8)));
    buffer[3] = (unsigned char)((p->checkSum & 0b11111111));
    buffer[4] = (unsigned char)((p->hops << 5) | p->type << 2 | p->ACK);
    
    if (p->headerLength == 6)
    {
        buffer[5] = (unsigned char)(p->srcCampus << 4 | p->destCampus);
        strcpy((char*)(buffer+6), p->data);
    }
    else
    {
        strcpy((char*)(buffer+5), p->data);
    }

    return buffer;
}

struct packet *deserialize(unsigned char* buffer){
    struct packet *p;
    p = malloc(sizeof(struct packet));

    p->version = (int)((buffer[0] & 0b11110000) >> 4);
    p->headerLength = (int)((buffer[0] & 0b00001111));
    p->totalLength = (int)(buffer[1]);
    p->srcDept = (int)((buffer[2] & 0b11100000) >> 5);
    p->destDept = (int)((buffer[2] & 0b00011100) >> 2);
    p->checkSum = (int)((buffer[2] & 0b00000011) << 8 | (buffer[3]));
    p->hops = (int)((buffer[4] & 0b11100000) >> 5);
    p->type = (int)((buffer[4] & 0b00011100) >> 2);
    p->ACK = (int)(buffer[4] & 0b00000011);
    
    if (p->headerLength == 6)
    {
        p->srcCampus = (int)((buffer[5] & 0b11110000) >> 4);
        p->destCampus = (int)((buffer[5] & 0b00001111));
        strcpy(p->data, (char*)(buffer+6));
    }
    else
    {
        strcpy(p->data, (char*)(buffer+5));
    }

    return p;
}

struct packet *generatePacket(int version, int headerLength, int totalLength,
                              int srcDept, int destDept, int checkSum, int hops,
                              int type, int ACK, int srcCampus, int destCampus, char* data) {
    //feel free to write your own function with more customisations. This is a very basic func
    struct packet *p;
    p = (struct packet *)malloc(sizeof(struct packet));
    p->version = version;
    p->headerLength = headerLength;
    p->totalLength = totalLength;
    p->srcDept = srcDept;
    p->destDept = destDept;
    p->checkSum = checkSum;
    p->hops = hops;
    p->type = type;
    p->ACK = ACK;
    p->srcCampus = srcCampus;
    p->destCampus = destCampus;
    strcpy(p->data, data);
    return p;
}

struct packet *makePacket(int headerLength, int srcDept, int destDept, int hops,
                        int type, int ACK, int srcCampus, int destCampus, char* data) 
{
    //feel free to write your own function with more customisations. This is a very basic func
    struct packet *p;
    p = (struct packet *)malloc(sizeof(struct packet));
    p->version = 1;
    p->headerLength = headerLength;
    p->totalLength = headerLength + strlen(data);
    p->srcDept = srcDept;
    p->destDept = destDept;
    p->checkSum = 0; // modify it for calculating checksum
    p->hops = hops;
    p->type = type;
    p->ACK = ACK;
    p->srcCampus = srcCampus;
    p->destCampus = destCampus;
    strcpy(p->data, data);
    return p;
}
