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

unsigned char* serialize(struct packet* p, int startLoc)
{
    unsigned char *buffer;
    buffer = malloc(p->totalLength + 2);
    int len = strlen(buffer);
    memset(buffer, 0, p->totalLength+2);

    buffer[startLoc + 0] = (unsigned char)((p->version << 4) | p->headerLength);
    buffer[startLoc + 1] = (unsigned char)(p->totalLength);
    buffer[startLoc + 2] = (unsigned char)((p->srcDept << 5 | p->destDept << 2 | ((p->checkSum & 0b1100000000) >> 8)));
    buffer[startLoc + 3] = (unsigned char)((p->checkSum & 0b11111111));
    buffer[startLoc + 4] = (unsigned char)((p->hops << 5) | p->type << 2 | p->ACK);
    
    if (p->headerLength == 6)
    {
        buffer[startLoc + 5] = (unsigned char)(p->srcCampus << 4 | p->destCampus);
        strncpy((char*)(buffer), p->data, startLoc);
        strcpy((char*)(buffer+startLoc+6), (char*)(p->data+startLoc));
    }
    else
    {
        strncpy((char*)(buffer), p->data, startLoc);
        strcpy((char*)(buffer+startLoc+5), (char*)(p->data+startLoc));
    }

    buffer[len-1] = (startLoc & 0xFF);
    buffer[len-2] = (startLoc & 0xFF00) >> 8;

    int sum = checksumCalc(buffer, startLoc);
    buffer[startLoc + 2] = (unsigned char)((p->srcDept << 5 | p->destDept << 2 | ((sum & 0b1100000000) >> 8)));
    buffer[startLoc + 2] = (unsigned char)((sum & 0b11111111));

    return buffer;
}

struct packet *deserialize(unsigned char* buffer)
{
    struct packet *p;
    p = malloc(sizeof(struct packet));

    int len = strlen(buffer);
    int startLoc = ((buffer[len-2] << 8) | buffer[len-1]);

    p->version = (int)((buffer[startLoc+0] & 0b11110000) >> 4);
    p->headerLength = (int)((buffer[startLoc+0] & 0b00001111));
    p->totalLength = (int)(buffer[startLoc+1]);
    p->srcDept = (int)((buffer[startLoc+2] & 0b11100000) >> 5);
    p->destDept = (int)((buffer[startLoc+2] & 0b00011100) >> 2);
    p->checkSum = (int)((buffer[startLoc+2] & 0b00000011) << 8 | (buffer[startLoc+3]));
    p->hops = (int)((buffer[startLoc+4] & 0b11100000) >> 5);
    p->type = (int)((buffer[startLoc+4] & 0b00011100) >> 2);
    p->ACK = (int)(buffer[startLoc+4] & 0b00000011);
    
    if (p->headerLength == 6)
    {
        p->srcCampus = (int)((buffer[startLoc+5] & 0b11110000) >> 4);
        p->destCampus = (int)((buffer[startLoc+5] & 0b00001111));
        strncpy(p->data, (char*)(buffer), startLoc);
        strcat(p->data, (char*)(buffer+startLoc+6));
    }
    else
    {
        strncpy(p->data, (char*)(buffer), startLoc);
        strcat(p->data, (char*)(buffer+startLoc+5));
    }

    return p;
}

int checksumCalc(unsigned char* buffer, int startLoc)
{
    int checksum = 0;
    for (int i=0; i<buffer[startLoc+1]; i++)
    {
        if (i==2)
            checksum += (buffer[startLoc+i] >> 2);
        else if (i==3)
            continue;
        else
            checksum += buffer[startLoc+i];
    }

    checksum = (~checksum) + 1;
    return checksum;
}

struct packet** processPacketServerSide(struct packet *p, int* validDept[3], int numOfValidDept[3], int deptPrivilegeLevels)
{
    struct packet** plist = malloc(2*sizeof(struct packet));
    struct packet* p1 = malloc(sizeof(struct packet));
    struct packet* p2 = malloc(sizeof(struct packet));

    if (p->srcCampus>2 || p->srcCampus<0 || p->destCampus>2 || p->destCampus<0)
    {
        plist[0] = NULL;
        p2 = makePacket(5, 0, p->srcDept, 1, 0, 1, p->srcCampus, p->srcCampus, "DST FIELDS INVALID 0");
        plist[1] = p2;
        return plist;
    }

    int srcDeptFound = 0;
    for (int i=0; i<numOfValidDept[p->srcCampus]; i++)
    {
        if (validDept[p->srcCampus][i] == p->srcDept)
            srcDeptFound = 1;
    }

    int destDeptFound = 0;
    for (int i=0; i<numOfValidDept[p->destCampus]; i++)
    {
        if (validDept[p->destCampus][i] == p->destDept)
            destDeptFound = 1;
    }
    
    if (!srcDeptFound || !destDeptFound)
    {
        plist[0] = NULL;
        p2 = makePacket(5, 0, p->srcDept, 1, 0, 1, p->srcCampus, p->srcCampus, "DST FIELDS INVALID 0");
        plist[1] = p2;
        return plist;
    }
    
    for (int i=0; i<(6-p->srcDept); i++)
    {
        deptPrivilegeLevels = deptPrivilegeLevels / 10;
    }

    int priv = deptPrivilegeLevels%10;

    

    if (priv==0)
    {
        p1 = makePacket(p->headerLength, p->srcDept, p->destDept, p->hops+1, p->type, 2, p->srcCampus, p->destCampus, p->data);
        p2 = makePacket(5, 0, p->srcDept, p->hops+1, p->type, 2, 0, p->srcCampus, "");
    }

    if (priv==1)
    {
        if (p->headerLength == 6 || p->destCampus==0b1111)
        {
            p1 = NULL;
            p2 = makePacket(5, 0, p->srcDept, 1, 0, 1, p->srcCampus, p->srcCampus, "PERMISSION DENIED 1");
        }
        else
        {
            p1 = makePacket(p->headerLength, p->srcDept, p->destDept, p->hops+1, p->type, 2, p->srcCampus, p->destCampus, p->data);
            p2 = makePacket(5, 0, p->srcDept, p->hops+1, p->type, 0, p->srcCampus, p->srcCampus, "");
        }
    }

    if (priv==2)
    {
        if (p->headerLength==6 || (p->srcCampus!=0 && p->destCampus!=0))
        {
            p1 = NULL;
            p2 = makePacket(5, 0, p->srcDept, 1, 0, 1, p->srcCampus, p->srcCampus, "PERMISSION DENIED 1");
        }
        else
        {
            p1 = NULL;
            p2 = makePacket(5, 0, p->srcDept, p->hops+1, p->type, 0, p->srcCampus, p->srcCampus, "");
        }
    }

    plist[0] = p1;
    plist[1] = p2;
    return plist;
}

unsigned char* getPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3])
{
    if (!strncmp(input, "1", 1) || !strncmp(input, "2", 1))
    {
        return generateUnicastPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else if (!strncmp(input, "3", 1) || !strncmp(input, "4", 1))
    {
        return generateBroadcastPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else if (!strncmp(input, "5", 1))
    {
        return generateControlPacket(input, srcCampus, srcDept, validDept, numOfValidDept);
    }
    else
    {
        return NULL;
    }
}

unsigned char* generateUnicastPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3])
{    
    if (srcCampus>2 || srcCampus<0)
        return NULL;

    int srcDeptFound = 0;
    for (int i=0; i<numOfValidDept[srcCampus]; i++)
    {
        if (validDept[srcCampus][i] == srcDept)
            srcDeptFound = 1;
    }
    
    if (!srcDeptFound)
        return NULL;
    
    struct packet* p = malloc(sizeof(struct packet));

    if (!strncmp(input, "1",1))
    {
        input += 2;
        char* token = strtok(input, ":");
        int destCampus = atoi(token);

        if (destCampus>2 || destCampus<0)
            return NULL;
        
        token = strtok(NULL, ":");
        int destDept = atoi(token);

        token = strtok(NULL, ":");
        char* payload = strdup(token);

        int destDeptFound = 0;
        for (int i=0; i<numOfValidDept[destCampus]; i++)
        {
            if (validDept[destCampus][i] == destDept)
                destDeptFound = 1;
        }
        if (!destDeptFound)
            return NULL;
        
        p = makePacket(6, srcDept, destDept, 0, 0, 2, srcCampus, destCampus, payload);
    }
    
    else if (!strncmp(input, "2",1))
    {
        input += 2;
        char* token = strtok(input, ":");
        int destDept = atoi(token);

        token = strtok(NULL, ":");
        char* payload = strdup(token);

        int destDeptFound = 0;
        for (int i=0; i<numOfValidDept[srcCampus]; i++)
        {
            if (validDept[srcCampus][i] == destDept)
                destDeptFound = 1;
        }
        if (!destDeptFound)
            return NULL;
        
        p = makePacket(5, srcDept, destDept, 0, 0, 2, srcCampus, 0, payload);
    }

    return serialize(p, 0);
}

unsigned char* generateBroadcastPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3])
{
    if (srcCampus>2 || srcCampus<0)
        return NULL;

    int srcDeptFound = 0;
    for (int i=0; i<numOfValidDept[srcCampus]; i++)
    {
        if (validDept[srcCampus][i] == srcDept)
            srcDeptFound = 1;
    }
    
    if (!srcDeptFound)
        return NULL;

    struct packet* p = malloc(sizeof(struct packet));

    int headerLength = 0;
    if (!strncmp(input, "3", 1))
        p = makePacket(5, srcDept, 0b111, 0, 0, 2, srcCampus, 0, input+2);
    else if (!strncmp(input, "4", 1))
        p = makePacket(6, srcDept, 0, 0, 0, 2, srcCampus, 0b1111, input+2);

    return serialize(p, 0);

}
 
unsigned char* generateControlPacket(char* input, int srcCampus, int srcDept, int* validDept[3], int numOfValidDept[3])
{
    if (srcCampus>2 || srcCampus<0)
        return NULL;

    int srcDeptFound = 0;
    for (int i=0; i<numOfValidDept[srcCampus]; i++)
    {
        if (validDept[srcCampus][i] == srcDept)
            srcDeptFound = 1;
    }
    
    if (!srcDeptFound)
        return NULL;

    struct packet* p = malloc(sizeof(struct packet));
    
    if (!strcmp(input, "5.EXIT\n"))
    {
        p = makePacket(5, srcDept, 0, 0, 1, 2, srcCampus, 0, "");
    }
    else
    {
        return NULL;
    }

    return serialize(p, 0);
}
 
struct packet *makePacket(int headerLength, int srcDept, int destDept, int hops, int type, int ACK, int srcCampus, int destCampus, char* data) 
{
    //feel free to write your own function with more customisations. This is a very basic func
    struct packet *p;
    p = (struct packet *)malloc(sizeof(struct packet));
    p->version = 1;
    p->headerLength = headerLength;
    p->totalLength = headerLength + strlen(data);
    p->srcDept = srcDept;
    p->destDept = destDept;
    p->hops = hops;
    p->type = type;
    p->ACK = ACK;
    p->srcCampus = srcCampus;
    p->destCampus = destCampus;
    strcpy(p->data, data);
    p->checkSum = 0;

    return p;
}
