#include <stdio.h>
#include "./impl/impl.h" //
#include <limits.h>

int main (int argc, char *argv[])
{
    struct packet* pkt = malloc(sizeof(struct packet));

    pkt->source_router = 0;
    pkt->dest_router = 1;
    pkt->num_entries = 2;

    pkt->distance_vector[0].target_router = 3;
    pkt->distance_vector[0].cost = 128;

    pkt->distance_vector[1].target_router = 2;
    pkt->distance_vector[1].cost = 12345;

    unsigned char* buffer = malloc(3 + (5*pkt->num_entries));
    memset(buffer, 0, 3 + (5*pkt->num_entries));
    buffer = serialize(pkt);
    int c = 3 + (5*pkt->num_entries);

    // while(c--){
    //     printf("0x%02x ", *(buffer++));
    // }
    // printf("\n");

    // pkt = deserialize(buffer);

    // printf("{\n");
    // printf("   SOURCE ROUTER: %d\n", pkt->source_router);
    // printf("   DESTINATION ROUTER: %d\n", pkt->dest_router);
    // printf("   NUM ENTRIES: %d\n", pkt->num_entries);
    // printf("   DISTANCE VECTORS: %d\n", pkt->distance_vector[1].cost);
    // printf("}\n");

    printf("%d\n", INT_MAX);

    return 0;
}