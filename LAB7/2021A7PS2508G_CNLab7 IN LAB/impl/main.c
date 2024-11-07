#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"


int main(int argc, char *argv[])
{   

    // TC 1
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "This is the first message\n");
    // p->version = 1;
    // p->headerLength = 5;
    // p->totalLength = 7 + strlen(p->data);
    // p->srcDept = 2;
    // p->destDept = 1;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;

    // unsigned char* stream = serialize(p, 3);

    // int c = 32;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 2
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "This is a broadcast message\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 2;
    // p->destDept = 7;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // p->srcCampus = 1;
    // p->destCampus = 15;

    // unsigned char* stream = serialize(p, 3);

    // int c = 35;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 3
    // unsigned char values[] = {
    //     84, 104, 105, 21, 32, 68, 0, 1, 115, 32, 
    //     105, 115, 32, 116, 104, 101, 32, 102, 105,
    //     114, 115, 116, 32, 109, 101, 115, 115, 97, 
    //     103, 101, 0, 3
    // };
    // size_t num_values = sizeof(values) / sizeof(values[0]);
    // unsigned char *stream = (unsigned char *)malloc(num_values * sizeof(unsigned char));
    // for (size_t i = 0; i < num_values; ++i) {
    //     stream[i] = values[i];
    // }
    // printPacket(deserialize(stream));

    // TC 5
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 1\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 3;
    // p->destDept = 4;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 0;
    // p->srcCampus = 0;
    // p->destCampus = 1;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] == NULL)
    // {
    //     printf("NULL\n");
    // }
    // printPacket(plist[1]);

    // TC 6
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 2\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 2;
    // p->destDept = 2;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 0;
    // p->srcCampus = 0;
    // p->destCampus = 2;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] == NULL)
    // {
    //     printf("NULL\n");
    // }
    // printPacket(plist[1]);

    // TC 7
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 3\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 3;
    // p->destDept = 1;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // p->srcCampus = 0;
    // p->destCampus = 1;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] == NULL)
    // {
    //     printf("NULL\n");
    // }
    // printPacket(plist[1]);

    // TC 8
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 4\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 3;
    // p->destDept = 7;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // p->srcCampus = 0;
    // p->destCampus = 15;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] != NULL)
    //     printPacket(plist[0]);
    // if (plist[1] != NULL)
    //     printPacket(plist[1]);

    // TC 9
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 5\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 4;
    // p->destDept = 7;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // p->srcCampus = 0;
    // p->destCampus = 15;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] != NULL)
    //     printPacket(plist[0]);
    // if (plist[1] != NULL)
    //     printPacket(plist[1]);

    // TC 10
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 6\0");
    // p->version = 1;
    // p->headerLength = 5;
    // p->totalLength = 7 + strlen(p->data);
    // p->srcDept = 2;
    // p->destDept = 0;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // int priv = 21020;

    // struct packet** plist = malloc(2*sizeof(struct packet));
    // plist = processPacketServerSide(p, validDept, numDept, priv);

    // if (plist[0] != NULL)
    //     printPacket(plist[0]);
    // if (plist[1] != NULL)
    //     printPacket(plist[1]);

    // TC 11
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "message 7\0");
    // p->version = 1;
    // p->headerLength = 6;
    // p->totalLength = 8 + strlen(p->data);
    // p->srcDept = 3;
    // p->destDept = 2;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;
    // p->srcCampus = 0;
    // p->destCampus = 2;

    // int pilani_dept[] = {1, 2, 3, 4};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {2, 3, 4, 5, 6}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {4, 2, 5};

    // TC 12
    struct packet* p = malloc(sizeof(struct packet));
    strcpy(p->data, "message 8\0");
    p->version = 1;
    p->headerLength = 5;
    p->totalLength = 7 + strlen(p->data);
    p->srcDept = 3;
    p->destDept = 4;
    p->checkSum = 0;
    p->hops = 0;
    p->type = 0;
    p->ACK = 1;

    int pilani_dept[] = {1, 2, 3, 4};
    int goa_dept[] = {1, 2};
    int hyd_dept[] = {2, 3, 4, 5, 6}; 
    int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    int numDept[] = {4, 2, 5};

    int priv = 21020;

    struct packet** plist = malloc(2*sizeof(struct packet));
    plist = processPacketServerSide(p, validDept, numDept, priv);

    if (plist[0] != NULL)
        printPacket(plist[0]);
    if (plist[1] != NULL)
        printPacket(plist[1]);

    // TC 4
    // char input[] = "2.3:Of";
    // int srcC = 1;
    // int srcD = 2;
    // int pilani_dept[] = {1, 2};
    // int goa_dept[] = {3, 4, 2};
    // int hyd_dept[] = {1, 2, 3, 4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {2, 3, 4};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 5
    // char input[] = "1.4:4:the\n";
    // int srcC = 3;
    // int srcD = 1;
    // int pilani_dept[] = {1};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {1, 2, 3}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 2, 3};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // if (stream==NULL)
    // {
    //     printf("NULL\n");
    // }


    // TC 6
    // char input[] = "2.3:4:Lambs\n";
    // int srcC = 2;
    // int srcD = 2;
    // int pilani_dept[] = {4};
    // int goa_dept[] = {4};
    // int hyd_dept[] = {4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 1, 1};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // if (stream==NULL)
    // {
    //     printf("NULL\n");
    // }

    // TC 7
    // char input[] = "3.TheGodFather";
    // int srcC = 1;
    // int srcD = 3;
    // int pilani_dept[] = {4};
    // int goa_dept[] = {1, 3};
    // int hyd_dept[] = {3, 4, 5}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 2, 3};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 8
    // char input[] = "4.Two\n";
    // int srcC = 0;
    // int srcD = 5;
    // int pilani_dept[] = {2, 4};
    // int goa_dept[] = {3, 4, 5};
    // int hyd_dept[] = {1, 2, 3, 4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {2, 3, 4};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // if (stream==NULL)
    // {
    //     printf("NULL\n");
    // }

    // TC 9
    // char input[] = "5.EXIT\n";
    // int srcC = 1;
    // int srcD = 2;
    // int pilani_dept[] = {1, 2};
    // int goa_dept[] = {1, 2, 3, 4};
    // int hyd_dept[] = {1, 2, 3, 4, 5}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {2, 4, 5};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 10
    // char input[] = "5.EXIT\n";
    // int srcC = 0;
    // int srcD = 1;
    // int pilani_dept[] = {3};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {1, 2, 3}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 2, 3};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // if (stream==NULL)
    // {
    //     printf("NULL\n");
    // }

    // TC 11
    // char input[] = "1.2:1:Hello";
    // int srcC = 0;
    // int srcD = 5;
    // int pilani_dept[] = {1, 3, 5};
    // int goa_dept[] = {2, 4};
    // int hyd_dept[] = {1, 2, 3}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {3, 2, 3};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 12
    // char input[] = "2.3:Test123\n";
    // int srcC = 1;
    // int srcD = 2;
    // int pilani_dept[] = {1, 2};
    // int goa_dept[] = {3, 4, 2};
    // int hyd_dept[] = {1, 2, 3, 4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {2, 3, 4};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 13
    // char input[] = "3.ShawShank\n";
    // int srcC = 2;
    // int srcD = 4;
    // int pilani_dept[] = {4};
    // int goa_dept[] = {4};
    // int hyd_dept[] = {4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 1, 1};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 14
    // char input[] = "4.Redemption\n";
    // int srcC = 2;
    // int srcD = 3;
    // int pilani_dept[] = {1, 3, 5};
    // int goa_dept[] = {3};
    // int hyd_dept[] = {5, 3, 4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 2, 4, 3, 5};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // printPacket(deserialize(stream));

    // unsigned char values[] = {
    //     38, 16, 127, 101, 0, 47, 82, 101, 100, 101, 109, 112, 116, 105, 111, 110
    // };
    // size_t num_values = sizeof(values) / sizeof(values[0]);
    // stream = (unsigned char *)malloc(num_values * sizeof(unsigned char));
    // for (size_t i = 0; i < num_values; ++i) {
    //     stream[i] = values[i];
    // }
    // printPacket(deserialize(stream));

    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 15
    // char input[] = "5.EXIT\n";
    // int srcC = 0;
    // int srcD = 1;
    // int pilani_dept[] = {1};
    // int goa_dept[] = {1, 2};
    // int hyd_dept[] = {1, 2, 3}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {1, 2, 3};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

    // TC 16
    // unsigned char values[] = {
    //     21, 32, 70, 119, 1, 84, 104, 105, 115, 32,
    //     105, 115, 32, 116, 104, 101, 32, 102, 105,
    //     114, 115, 116, 32, 109, 101, 115, 115, 97, 103, 101
    // };
    // size_t num_values = sizeof(values) / sizeof(values[0]);
    // unsigned char *stream = (unsigned char *)malloc(num_values * sizeof(unsigned char));
    // for (size_t i = 0; i < num_values; ++i) {
    //     stream[i] = values[i];
    // }

    // printf("%d\n", checksumCalc(stream));


    

    return 0;
}