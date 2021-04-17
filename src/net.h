#ifndef INC_NET_H
#define INC_NET_H

#include "layer.h"
#include "poollist.h"
#include "errorc.h"

#define LAYER_POOL_SIZE 64


extern struct p_root all_layers, all_layer_connections;

struct n_network {
    int in_size, out_size;
    int input_layer;
    int output_layer;
    int biggest_size;

    struct pl_pool_list layers;

    float *output_buffer;
};

void n_network_init(struct n_network *net, int in_size, int out_size);
void n_network_deinit(struct n_network *net);
int n_network_add_layer(struct n_network *net, struct l_layer_type *type, f_activation activation, int in_size, int out_size);
int n_network_connect_layers(struct n_network *net, int input, int output);

error_code_t n_network_process(struct n_network *net, float *inputs);
error_code_t n_network_process_to(struct n_network *net, float *inputs, float *outputs);


#endif // INC_NET_H