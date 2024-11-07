#include "impl.h"
#include <stdio.h>

// src 8, dest 8, num_ent 8, dist_vec (var)
unsigned char* serialize(struct packet* pkt) {
    /* Todo: Please write the code here*/
    unsigned char *buffer;
    buffer=malloc(3+pkt->num_entries*5 );
    buffer[0]=(unsigned char)pkt->source_router;
    buffer[1]=(unsigned char)pkt->dest_router;
    buffer[2]=(unsigned char)pkt->num_entries;
    int idx=3;int dv_idx=0;
    int num_entries=buffer[2];
    while(num_entries--){
        buffer[idx++]=(unsigned char)pkt->distance_vector[dv_idx].target_router;
        buffer[idx++]=(unsigned char)pkt->distance_vector[dv_idx].cost>>24 & 0xFF;
        buffer[idx++]=(unsigned char)pkt->distance_vector[dv_idx].cost>>16 & 0xFF;
        buffer[idx++]=(unsigned char)pkt->distance_vector[dv_idx].cost>>8 & 0xFF;
        buffer[idx++]=(unsigned char)pkt->distance_vector[dv_idx].cost & 0xFF;
        dv_idx++;
    }
    return buffer;
}
    
struct packet *deserialize(unsigned char* buffer) {
    struct packet* pkt = (struct packet*) malloc( sizeof(struct packet));
    pkt->source_router=(int)buffer[0];
    pkt->dest_router=(int)buffer[1];
    pkt->num_entries=(int)buffer[2];
    
    int idx=3;
    int num_entries=pkt->num_entries;
    int dv_idx=0;
    while(num_entries--){
        pkt->distance_vector[dv_idx].target_router=buffer[idx++];
        pkt->distance_vector[dv_idx].cost=(buffer[idx++]<<24) | (buffer[idx++]<<16) | (buffer[idx++]<<8) | (buffer[idx++]);
        dv_idx++;
    }
    return pkt;
}

void router_init(struct distance_table *my_dt, int my_node, int *my_link_costs, int num_nodes)
{
    for( int i = 0; i < num_nodes; i++){
        for( int j = 0; j < num_nodes; j++){
            if( i == j){
                my_dt->costs[i][j] = 0;
            }else{
                my_dt->costs[i][j] = -1;
            }
        }
    }

    for( int i = 0; i < num_nodes; i++ ){
        my_dt->next_hop[i] = -1;
    }

    // initialise the direct neighbours' costs
    for( int i = 0; i < num_nodes; i++ ){
        if( my_link_costs[i] != -1){
            my_dt->costs[my_node][i] = my_link_costs[i];
            my_dt->next_hop[i] = i;
        }
        else{ // next_hop = -1 for same node or unreachable node
            my_dt->next_hop[i] = -1;
        }
    }

    /*
    A. Allocate memory for the packet
    B. Fill in appropriate values for the source_router=0 and dest_router=1 in the header
    C. For each positive cost in node 0’s distance vector, add a dv_entry to the distance_vector
    array and increment total_entries. Here, total_entries will be 2, and the distance_vector array
    will look like [{target_router: 1, cost: 20}, {target_router: 3, cost: 7}]
    D. Now use serialize to return a pointer to the packet_buffer, and call send2neighbor
    */

    // send packets to neighbours

    struct packet* pkt_ptr = (struct packet*)malloc( sizeof(struct packet));
    pkt_ptr->source_router = my_node;

    int num_neighs = 0;
    int neigh_nodes[num_nodes] ;
    int costs_to_neigh[num_nodes];

    for( int j = 0; j < num_nodes; j++){
        if( my_link_costs[j] != -1 && j != my_node){
            neigh_nodes[num_neighs] = j;
            costs_to_neigh[num_neighs] = my_link_costs[j];
            num_neighs++;
        }
    }
    
    pkt_ptr->num_entries = num_neighs;
    for( int i = 0; i < num_neighs; i++){
        int neigh_node = neigh_nodes[i];
        int cost = costs_to_neigh[i];
        pkt_ptr->distance_vector[i].target_router = neigh_node;
        pkt_ptr->distance_vector[i].cost = cost;
    }

    // send the packet to neighbours
    for( int i = 0; i < num_neighs; i++){
        int neigh_node = neigh_nodes[i];
        pkt_ptr->dest_router = neigh_node;
        unsigned char* buffer = serialize( pkt_ptr);
        send2neighbor(buffer);
    }
    free( pkt_ptr );
}

void router_update(struct distance_table *my_dt, int my_node, unsigned char* packet_buffer, int *my_link_costs, int num_nodes)
{
    int INT_MAX = 69696969;
    struct packet* pkt = deserialize(packet_buffer);
    int src_router = pkt->source_router;
    // there was some change in DVT
    
    // initialize the src's dist vector to [ -1 -1 -1 ... 0... -1 -1 -1 ]
    // for( int i = 0; i < num_nodes; i++){
    //     if( src_router != i){
    //         // printf("src_router: %d, i: %d\n", src_router, i);
    //         my_dt->costs[src_router][i] = -1;
    //     }
    //     else{
    //         my_dt->costs[src_router][i] = 0;
    //     }
    // }

    // update source router's dist in local table
    for( int i = 0; i < pkt->num_entries; i++){
        int target = pkt->distance_vector[i].target_router;
        int cost = pkt->distance_vector[i].cost;
        my_dt->costs[src_router][target] = cost;
    }
    free(pkt);

    // if there is change in my_link_costs, it is handled in the my_link_costs array

    // made update the local dvt
    int updated = 0;
    for( int tar = 0; tar < num_nodes; tar++ ){
        if( tar != my_node ){
            int min_cost = INT_MAX;
            int min_cost_node = -1;
            for( int neig = 0; neig < num_nodes; neig++ ){
                if( my_link_costs[neig] != -1 && neig != my_node && my_dt->costs[neig][tar] != -1){
                    // found a neighbour and neighbour has a known path to target
                    int prop_cost = my_link_costs[neig] + my_dt->costs[neig][tar];
                    if( prop_cost < min_cost ){
                        min_cost = prop_cost;
                        min_cost_node = neig;
                    }
                }
            }
            int curr_cost = my_dt->costs[my_node][tar];
            if( min_cost == INT_MAX ){
                min_cost = -1;
            }
            if( min_cost != curr_cost ){
                my_dt->costs[my_node][tar] = min_cost;
                my_dt->next_hop[tar] = min_cost_node;
                updated = 1;
            }
        }
    }

    // spam my table to neighbours
    if( updated ){
        // send packets to neighbours
        struct packet pkt;
        pkt.source_router = my_node;

        int num_neighs = 0;
        int neigh_nodes[num_nodes] ;
        int costs_to_neigh[num_nodes];

        for( int j = 0; j < num_nodes; j++){
            if( my_link_costs[j] != -1 && j != my_node){
                neigh_nodes[num_neighs] = j;
                costs_to_neigh[num_neighs] = my_link_costs[j];
                num_neighs++;
            }
        }

        

        // send the packet to neighbours
        for( int i = 0; i < num_neighs; i++){
            int neigh_node = neigh_nodes[i];
            pkt.dest_router = neigh_node;
            pkt.num_entries = num_nodes;
            
            for( int i = 0; i < num_nodes; i++){
                int cost = my_dt->costs[my_node][i];
                pkt.distance_vector[i].target_router = i;
                int my_next_hop_to_go_tar = my_dt->next_hop[i];
                if( my_next_hop_to_go_tar == neigh_node ){
                    pkt.distance_vector[i].cost = -1;
                }
                else{
                    pkt.distance_vector[i].cost = cost;
                }
            }
            // printf("%d:\t Sending packet to %d\n", my_node , neigh_node);
            // print_packet(&pkt);
            unsigned char* buffer = serialize(&pkt);
            send2neighbor(buffer);
        }

    }
}

/*
* Function which returns the next hop router index based on the current local distance
table and the destination router index.
This function should return -1 when the current router knows no path to the destination.
*/
int route_packet(struct distance_table *my_dt, int dest_node){
    return my_dt->next_hop[dest_node];
}

/*
This function is called whenever there is an update in a link’s cost.
a. my_node specifies the router that is being notified about the change.
b. changed_index specifies the index in my_link_costs ,which holds the
updated value.
You will have to modify the local distance table my_dt to hold the new optimal path to all
the nodes. If there is any change in the distance vector, appropriate calls to
send2neighbor should be made to propagate the change, like in the case of
router_update.
*/
void router_link_update(struct distance_table *my_dt, int my_node, int changed_index, int *my_link_costs, int num_nodes){
    /* Todo: Please write the code here*/
    struct packet* pkt_ptr = ( struct packet* )malloc( sizeof(struct packet) );
    pkt_ptr->source_router = -1;
    pkt_ptr->dest_router = my_node;
    pkt_ptr->num_entries = 0;
    router_update( my_dt, my_node, serialize(pkt_ptr), my_link_costs, num_nodes);
}

