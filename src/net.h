#ifndef INC_NET_H
#define INC_NET_H

#include "layer.h"
#include "pool.h"

#define LAYER_POOL_SIZE 64


extern struct p_root all_layers;

struct n_layer_connection {
    int from;
    int to;
};

struct n_network {
    struct p_root layers;
    struct p_root layer_connections;
};


#endif // INC_NET_H