#include <string.h>

#include "net.h"
#include "poollist.h"
#include "layer.h"
#include "errorc.h"


struct p_root all_layers = POOL_ROOT(struct l_layer, LAYER_POOL_SIZE);
struct p_root all_connections = POOL_ROOT(struct n_connection, LAYER_POOL_SIZE);

void n_network_init(struct n_network *net, int in_size, int out_size) {
    pl_initialize(&net->layers, &all_layers, 16);
    pl_initialize(&net->connections, &all_connections, 32);

    net->in_size = in_size;
    net->out_size = out_size;

    net->input_layer = -1;
    net->output_layer = -1;

    net->biggest_size = in_size > out_size ? in_size : out_size;

    net->output_buffer = NULL;
}

void n_network_deinit(struct n_network *net) {
    for (struct pl_iter li = pl_iterate(&net->layers, 0, -1); pl_next(&li);) {
        l_layer_deinit(li.item);
    }

    pl_deinit(&net->layers);
}

int n_network_add_layer(struct n_network *net, struct l_layer_type *type, f_activation activation, int in_size, int out_size) {
    int idx = 0;

    struct l_layer *new_layer = pl_allocate(&net->layers, &idx);
    l_layer_init(new_layer, type, activation, in_size, out_size);

    int biggest = in_size > out_size ? in_size : out_size;

    if (biggest > net->biggest_size) {
        net->biggest_size = biggest;
    }

    return idx;
}

static void n_network_register_conn(struct n_network *net, int input, int output) {
    struct n_connection *item = pl_allocate(&net->connections, NULL);
    item->from = input;
    item->to = output;
}

error_code_t n_network_connect_layers(struct n_network *net, int input, int output) {
    if (input == -1 && output == -1) {
        ERR_RET(ES_NET_CONN_INPUT_OUTPUT);
    }

    if (input == -1) {
        struct l_layer *layer = pl_get(&net->layers, output);

        if (layer->in_size != net->in_size) {
            ERR_RET(ES_NET_CONN_NET_SIZE_MISMATCH);
        }

        net->input_layer = output;

        n_network_register_conn(net, input, output);

        return -1;
    }

    if (output == -1) {
        struct l_layer *layer = pl_get(&net->layers, input);

        if (layer->out_size != net->out_size) {
            ERR_RET(ES_NET_CONN_NET_SIZE_MISMATCH);
        }

        net->output_layer = input;
        net->output_buffer = layer->output_buffer;

        n_network_register_conn(net, input, output);

        return -1;
    }

    struct l_layer *from_layer = pl_get(&net->layers, input);
    struct l_layer *to_layer = pl_get(&net->layers, output);

    if (from_layer->out_size + to_layer->in_used > to_layer->in_size) {
        ERR_RET(ES_NET_CONN_LAYER_EXCEEDS);
    }

    from_layer->output_buffer = to_layer->input_buffer + to_layer->in_used;
    to_layer->in_used += from_layer->out_size;

    n_network_register_conn(net, input, output);

    SUCCESS;
}

error_code_t n_network_process(struct n_network *net, float *inputs) {
    struct l_layer *layer;

    if (net->input_layer == -1) {
        ERR_RET(ES_NET_PROC_NO_IN_LAYER);
    }

    layer = pl_get(&net->layers, net->input_layer);
    memcpy(layer->input_buffer, inputs, sizeof(float) * layer->in_size);

    for (struct pl_iter li = pl_iterate(&net->layers, 0, -1); pl_iter_has(&li); pl_next(&li)) {
        layer = li.item;
        l_layer_process(layer);
    }

    SUCCESS;
}

error_code_t n_network_process_to(struct n_network *net, float *inputs, float *outputs) {
    CALL_RET(n_network_process(net, inputs));

    if (net->output_buffer == NULL) {
        ERR_RET(ES_NET_PROC_NO_IN_LAYER);
    }

    memcpy(outputs, net->output_buffer, sizeof(float) * net->out_size);

    SUCCESS;
}

void n_network_copy(struct n_network *dest, struct n_network *net) {
    n_network_init(dest, net->in_size, net->out_size);

    for (struct pl_iter li = pl_iterate(&net->layers, 0, -1); pl_iter_has(&li); pl_next(&li)) {
        struct l_layer *src_layer = li.item;
        int dest_li = n_network_add_layer(dest, src_layer->type, src_layer->activation, src_layer->in_size, src_layer->out_size);

        struct l_layer *dest_layer = pl_get(&dest->layers, dest_li);
        memcpy(dest_layer->weights, src_layer->weights, sizeof(float) * src_layer->num_weights);
    }

    for (struct pl_iter ci = pl_iterate(&net->connections, 0, -1); pl_iter_has(&ci); pl_next(&ci)) {
        struct n_connection *conn = ci.item;
        n_network_connect_layers(dest, conn->from, conn->to);
    }
}
