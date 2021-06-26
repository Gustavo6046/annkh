#pragma once

#include "layer.h"
#include "poollist.h"
#include "errorc.h"

#define LAYER_POOL_SIZE 64


struct n_connection {
    int from, to;
};

extern struct p_root all_layers, all_connections;

struct n_network {
    int in_size, out_size;
    int input_layer;
    int output_layer;
    int biggest_size;

    struct pl_pool_list layers;
    struct pl_pool_list connections;

    float *output_buffer;
};

void n_network_init(struct n_network *net, int in_size, int out_size);
void n_network_deinit(struct n_network *net);
void n_network_copy(struct n_network *dest, struct n_network *net);
int n_network_add_layer(struct n_network *net, struct l_layer_type *type, f_activation activation, int in_size, int out_size, struct l_layer **set_layer);
error_code_t n_network_connect_layers(struct n_network *net, int input, int output);

error_code_t n_network_process(struct n_network *net, float *inputs);
error_code_t n_network_process_to(struct n_network *net, float *inputs, float *outputs);
