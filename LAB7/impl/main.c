#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"


int main(int argc, char *argv[])
{   
    // // char message[] = "1.2:1:Hello\n";
    // // char message[] = "2.3:Hello";
    // // char message[] = "2.4:Hello";
    // // char message[] = "4.wrrwerwrw\n";
    // // char message[] = "4.hehe\n";
    // // char message[] = "5.EXIT\n";
    // char message[] = "5.EXITds\n";
    // int srcCampus = 0;
    // int srcDept = 5;
    // int* validDept[3] = {
    //             (int[]) {1, 3, 5}, 
    //             (int[]) {2, 4}, 
    //             (int[]) {1, 2, 3}
    //         };
    // int numOfValidDept[3] = {3,2,3};

    // unsigned char* stream = getPacket(message, srcCampus, srcDept, validDept, numOfValidDept);

    // if (stream)
    //     printPacket(deserialize(stream));
    // else
    //     printf("NULL\n");

    // TC 1
    // struct packet* p = malloc(sizeof(struct packet));
    // strcpy(p->data, "This is the first message\0");
    // p->version = 1;
    // p->headerLength = 5;
    // p->totalLength = p->headerLength + strlen(p->data);
    // p->srcDept = 2;
    // p->destDept = 1;
    // p->checkSum = 0;
    // p->hops = 0;
    // p->type = 0;
    // p->ACK = 1;

    // unsigned char* stream = serialize(p);

    // int c = 30;
    // while(c--)
    // {
    //     printf("%02d ", *(stream++));
    // }

    // TC 2
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
    // printPacket(deserialize(stream));

    // TC 3
    // char input[] = "1.0:5:Silence\n";
    // int srcC = 2;
    // int srcD = 4;
    // int pilani_dept[] = {1, 3, 5};
    // int goa_dept[] = {2, 4};
    // int hyd_dept[] = {1, 2, 3, 4}; 
    // int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    // int numDept[] = {3, 2, 4};

    // unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    // int c = 30;
    // while(c--)
    // {
    //     printf("%d ", *(stream++));
    // }

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
    char input[] = "4.Redemption\n";
    int srcC = 2;
    int srcD = 3;
    int pilani_dept[] = {1, 3, 5};
    int goa_dept[] = {3};
    int hyd_dept[] = {5, 3, 4}; 
    int* validDept[] = {pilani_dept, goa_dept, hyd_dept};
    int numDept[] = {1, 2, 4, 3, 5};

    unsigned char* stream = getPacket(input, srcC, srcD, validDept, numDept);
    printPacket(deserialize(stream));

    unsigned char values[] = {
        38, 16, 127, 101, 0, 47, 82, 101, 100, 101, 109, 112, 116, 105, 111, 110
    };
    size_t num_values = sizeof(values) / sizeof(values[0]);
    stream = (unsigned char *)malloc(num_values * sizeof(unsigned char));
    for (size_t i = 0; i < num_values; ++i) {
        stream[i] = values[i];
    }
    printPacket(deserialize(stream));

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