#include "impl.h"
#include <stdio.h>

unsigned char* serialize(struct packet* pkt) {
    /* Todo: Please write the code here*/
    unsigned char *buffer;
    buffer = malloc(3 + (5*pkt->num_entries));
    memset(buffer, 0, 3 + (5*pkt->num_entries));

    buffer[0] = (unsigned char)((pkt->source_router));
    buffer[1] = (unsigned char)((pkt->dest_router));
    buffer[2] = (unsigned char)((pkt->num_entries));

    for (int i=0; i<(pkt->num_entries); i++)
    {
        buffer[3 + (5*i) + 0] = pkt->distance_vector[i].target_router;
        buffer[3 + (5*i) + 1] = (pkt->distance_vector[i].cost & 0xFF000000) >> 24;
        buffer[3 + (5*i) + 2] = (pkt->distance_vector[i].cost & 0x00FF0000) >> 16;
        buffer[3 + (5*i) + 3] = (pkt->distance_vector[i].cost & 0x0000FF00) >> 8;
        buffer[3 + (5*i) + 4] = (pkt->distance_vector[i].cost & 0x000000FF) >> 0;
    }

    return buffer;
}

struct packet *deserialize(unsigned char* buffer) {
    /* Todo: Please write the code here*/
    struct packet* pkt;
    pkt = malloc(sizeof(struct packet));

    pkt->source_router = (int)buffer[0];
    pkt->dest_router = (int)buffer[1];
    pkt->num_entries = (int)buffer[2];

    for (int i=0; i<(pkt->num_entries); i++)
    {
        pkt->distance_vector[i].target_router = buffer[3 + (5*i) + 0];
        pkt->distance_vector[i].cost = 
                        buffer[3 + (5*i) + 1] << 24 |
                        buffer[3 + (5*i) + 2] << 16 |
                        buffer[3 + (5*i) + 3] << 8  |
                        buffer[3 + (5*i) + 4] << 0;
    }

    return pkt;
}

void router_init(struct distance_table *my_dt, int my_node, int *my_link_costs, int num_nodes)
{
    /* Todo: Please write the code here*/

    struct packet* pkt = malloc(sizeof(struct packet));
    pkt->source_router = my_node;
    int count = 0;

    for (int i=0; i<num_nodes; i++)
    {
        my_dt->costs[my_node][i] = my_link_costs[i];
        if (my_dt->costs[my_node][i] > 0)
        {
            pkt->distance_vector[count].target_router = i;
            pkt->distance_vector[count].cost = my_dt->costs[my_node][i];
            count++;
        } 
    }

    pkt->num_entries = count;

    for (int i=0; i<num_nodes; i++)
    {
        if (my_link_costs[i] != -1 && i!=my_node)
        {
            pkt->dest_router = i;
            send2neighbor(serialize(pkt));
        }
    }
}

void router_update(struct distance_table *my_dt, int my_node, unsigned char* packet_buffer, int *my_link_costs, int num_nodes)
{
    /* Todo: Please write the code here*/
    struct packet* pkt = malloc(sizeof(struct packet));
    pkt = deserialize(packet_buffer);

    for (int i=0; i<pkt->num_entries; i++)
    {
        my_dt->costs[pkt->source_router][pkt->distance_vector[i].target_router] = pkt->distance_vector[i].cost;
    }

    struct packet* new_pkt = malloc(sizeof(struct packet));
    int count = 0;

    for (int y=0; y<num_nodes; y++)
    {
        for (int v=0; v<num_nodes; v++)
        {
            if (my_link_costs[v]!=-1 && my_dt->costs[v][y]!=-1)
            {
                if ((my_dt->costs[my_node][y] > my_link_costs[v] + my_dt->costs[v][y]) || my_dt->costs[my_node][y]==-1)
                {
                    my_dt->costs[my_node][y] = my_link_costs[v] + my_dt->costs[v][y];
                    new_pkt->distance_vector[count].target_router = y;
                    new_pkt->distance_vector[count].cost = my_dt->costs[my_node][y];
                    count++;
                } 
            }
        }
    }

    new_pkt->source_router = my_node;
    new_pkt->num_entries = count;

    for (int i=0; i<num_nodes; i++)
    {
        if (my_link_costs[i] != -1 && i!=my_node && count>0)
        {
            new_pkt->dest_router = i;
            send2neighbor(serialize(new_pkt));
        }
    }
}
